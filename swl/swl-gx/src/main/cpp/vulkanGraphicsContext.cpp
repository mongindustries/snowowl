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

	for (const auto &physicalDevice : devices) {

		auto properties = physicalDevice.getProperties();
		cout << "\t" << properties.deviceName << endl;

		_active_device = physicalDevice;
		break;
	}

	cout << "VulkanGraphicsContext: Device created!" << endl;
}

VulkanGraphicsContext::~VulkanGraphicsContext() {

	_instance.release().destroy();

	if (loader) {
		delete loader;
	}
}

VulkanGraphicsContext::VulkanGraphicsContext(VulkanGraphicsContext&& mov) noexcept:
	loader(mov.loader),
	_instance(move(mov._instance)),
	_active_device(move(mov._active_device)) {
	mov.loader = nullptr;
}

VulkanGraphicsContext& VulkanGraphicsContext::operator=(VulkanGraphicsContext&& mov) noexcept {

	loader = mov.loader;
	_instance = move(mov._instance);
	_active_device = move(mov._active_device);
	mov.loader = nullptr;
	
	return *this;
}

void VulkanGraphicsContext::makeSurface(ui::WindowSurface &surface) const {
	// to backend
	backend::VulkanGraphicsBackend::instance->makeSurface(_instance.get(), surface);
}

vk::SurfaceKHR const& VulkanGraphicsContext::getSurface(const ui::WindowSurface &surface) const {
	return backend::VulkanGraphicsBackend::instance->surfaces.at(reference_wrapper(const_cast<ui::WindowSurface&>(surface)));
}
