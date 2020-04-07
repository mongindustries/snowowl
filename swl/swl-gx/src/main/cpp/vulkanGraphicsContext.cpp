//
// Created by Michael Ong on 6/4/20.
//
#include "vulkanGraphicsContext.hpp"
#include "vulkanGraphicsBackend.hpp"

#include <iostream>

using namespace swl::cx;

using namespace swl::gx;
using namespace swl::gx::implem;

using namespace std;

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE;

VulkanGraphicsContext::VulkanGraphicsContext() {

	vk::DynamicLoader loader;
	PFN_vkGetInstanceProcAddr getInstanceProcAddr = loader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
	VULKAN_HPP_DEFAULT_DISPATCHER.init(getInstanceProcAddr);

	vk::ApplicationInfo appInfo(
		"App", 0x00'01'00,
		"SnowOwl:", 0x00'01'00,
		VK_API_VERSION_1_1);

	auto& extensions = backend::VulkanGraphicsBackend::vulkanExtensions;
	vk::InstanceCreateInfo instanceCreateInfo({},
		&appInfo,
		0, nullptr,
		extensions.size(), extensions.data());

	_instance = vk::createInstanceUnique(instanceCreateInfo);
	VULKAN_HPP_DEFAULT_DISPATCHER.init(_instance.get());

	cout << "VulkanGraphicsContext: Instance created!" << endl;

	const auto devices = _instance->enumeratePhysicalDevices();
	cout << "VulkanGraphicsContext: " << devices.size() << " devices found!" << endl;

	for (const auto &physicalDevice : devices) {

		auto properties = physicalDevice.getProperties();
		cout << "\t" << properties.deviceName << endl;

		auto queueProps = physicalDevice.getQueueFamilyProperties();

		auto index = std::distance(queueProps.begin(),
		              std::find_if(queueProps.begin(), queueProps.end(),
                    [](const vk::QueueFamilyProperties& item) { return item.queueFlags & (vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eTransfer); }));

		vk::DeviceQueueCreateInfo deviceQueueCreateInfo({},
			index,
			1,
			array<float, 1> { 0 }.data());

		vk::DeviceCreateInfo deviceCreateInfo({},
			1,
			array<vk::DeviceQueueCreateInfo, 1> { deviceQueueCreateInfo }.data());

		_active_device = physicalDevice.createDeviceUnique(deviceCreateInfo);
		break;
	}

	cout << "VulkanGraphicsContext: Device created!" << endl;
}

void VulkanGraphicsContext::makeSurface(ui::WindowSurface &surface) {
	std::cout << "vulkan create surface" << std::endl;

	// to backend
	backend::VulkanGraphicsBackend::instance->makeSurface(_instance.get(), surface);
}
