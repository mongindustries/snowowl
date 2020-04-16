//
// Created by Michael Ong on 6/4/20.
//
#include "vulkanGraphicsContext.hpp"
#include "vulkan_graphics_backend.hpp"
#include "vulkanGraphicsSwapChain.hpp"
#include "vulkanGraphicsQueue.hpp"

#include <iostream>

using namespace swl::cx;
using namespace swl::gx;

using namespace std;

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE;

VulkanGraphicsContext::VulkanGraphicsContext(VulkanGraphicsContext&& mov) noexcept:
	_instance      (move(mov._instance)),
	_active_device (mov._active_device),
	_device        (move(mov._device)),
	loader         (mov.loader) {
}

VulkanGraphicsContext& VulkanGraphicsContext::operator=(VulkanGraphicsContext&& mov) noexcept {

	_instance       = move(mov._instance);
	_active_device  = mov._active_device;
	_device         = move(mov._device);
	loader          = mov.loader;

	return *this;
}

// implem

VulkanGraphicsContext::VulkanGraphicsContext(): loader(new vk::DynamicLoader()) {

	const auto getInstanceProcAddr = loader->getProcAddress<PFN_vkGetInstanceProcAddr>(
		"vkGetInstanceProcAddr");
	VULKAN_HPP_DEFAULT_DISPATCHER.init(getInstanceProcAddr);

	vk::ApplicationInfo appInfo(
		"App", 0x00'01'00,
		"SnowOwl:", 0x00'01'00,
		VK_API_VERSION_1_1);

	auto& extensions = backend::VulkanGraphicsBackend::vulkanExtensions;
	auto& layers = backend::VulkanGraphicsBackend::vulkanLayers;

	vk::InstanceCreateInfo instanceCreateInfo({},
	                                          &appInfo,
	                                          layers.size(), layers.data(),
	                                          extensions.size(), extensions.data());

	_instance = vk::createInstanceUnique(instanceCreateInfo);
	VULKAN_HPP_DEFAULT_DISPATCHER.init(_instance.get());

	cout << "VulkanGraphicsContext: Instance created!" << endl;

	const auto devices = _instance->enumeratePhysicalDevices();
	cout << "VulkanGraphicsContext: " << devices.size() << " devices found!" << endl;

	_active_device  = devices.back();

	auto properties = _active_device.getProperties();
	cout << "\t" << properties.deviceName << endl;

	cout << "VulkanGraphicsContext: Device created!" << endl;
}

void
	VulkanGraphicsContext::createDevice(const vector<MutableBorrow<VulkanGraphicsQueue>> &queues) {

	vector<vk::DeviceQueueCreateInfo> queueInfo(queues.size());

	uint32_t index = 0;
	float priorities = 1.0f;

	for (const auto &queue: queues) {
		queueInfo[index].queueFamilyIndex = queue.get().familyIndex;
		queueInfo[index].queueCount = 1;
		queueInfo[index].pQueuePriorities = &priorities;
	}

	auto enabledExts = std::vector<const char*> {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	vk::DeviceCreateInfo createDevice{};

	createDevice.queueCreateInfoCount = queueInfo.size();
	createDevice.pQueueCreateInfos = queueInfo.data();

	createDevice.enabledExtensionCount = enabledExts.size();
	createDevice.ppEnabledExtensionNames = enabledExts.data();

	_device = _active_device.createDeviceUnique(createDevice);
	VULKAN_HPP_DEFAULT_DISPATCHER.init(_device.get());

	for (auto &queue: queues) {

		queue.get().prepare(_device.get());
		assert(queue.get().isReady());
	}
}
