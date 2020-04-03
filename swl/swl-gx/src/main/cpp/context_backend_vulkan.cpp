//
// Created by Michael Ong on 1/4/20.
//

#include "context_backend_vulkan.hpp"

#define SWL_WIN32

#if defined(SWL_DARWIN)
	#include "swl_vulkan_mac.hpp"
#elif defined(SWL_WIN32)
	#include "swl_vulkan_win32.hpp"
#endif

#include "swl_vulkan.hpp"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

using namespace std;

using namespace swl::cx;

using namespace swl::gx::internal;
using namespace swl::gx::backend;

struct ContextBackendVulkan::Context {
	vk::DynamicLoader loader;
	vk::Instance instance;

	vk::PhysicalDevice physicalDevice;
	vk::Device device;
};

void loadLoader(vk::DynamicLoader &loader) {

	auto vkGetInstanceProcAddr = loader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
	VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

	vk::Instance instance = vk::createInstance({ }, nullptr);
	VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);
}

ContextBackendVulkan::ContextBackendVulkan(): context(new Context()) {

	loadLoader(context().loader);

	vk::ApplicationInfo applicationInfo {
		"snowowl: app",
		0x01'00'00,
		"snowowl:",
		0x01'00'00,
		VK_API_VERSION_1_0
	};

	auto extensions = VulkanPlatform::extensions_c_const();

	vk::InstanceCreateInfo instanceCreateInfo {
			{},
			&applicationInfo,
			0,
			{ },
			(uint32_t) extensions.size(),
			extensions.data(),
	};

	auto& ctx = context();
	ctx.instance = vk::createInstance(instanceCreateInfo);

	for (auto item : ctx.instance.enumeratePhysicalDevices()) {
		auto prop = item.getProperties();

		if (prop.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
			ctx.physicalDevice = item;
		}
	}

	vk::DeviceCreateInfo deviceCreateInfo {

	};

	ctx.device = ctx.physicalDevice.createDevice(deviceCreateInfo);
}

string ContextBackendVulkan::name() const {
	return "vulkan";
}

ContextBackendVulkan::ContextBackendVulkan(ContextBackendVulkan &&mov) noexcept: context(mov.context) {
	mov.context.invalidate();
}
