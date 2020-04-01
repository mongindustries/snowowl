//
// Created by Michael Ong on 1/4/20.
//

#include "device_backend_vulkan.hpp"

#include "swl_vulkan_mac.hpp"
#include "swl_vulkan.hpp"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

using namespace std;

using namespace swl::cx;

using namespace swl::gx_private;
using namespace swl::gx::backend;

struct DeviceBackendVulkan::Context {
	vk::DynamicLoader loader;
	vk::Instance instance;
};

void loadLoader(vk::DynamicLoader &loader) {

	PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = loader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
	VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

	vk::Instance instance = vk::createInstance({ }, nullptr);
	VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);
}

DeviceBackendVulkan::DeviceBackendVulkan(): context(new Context()) {

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
}

string DeviceBackendVulkan::name() const {
	return "vulkan";
}

DeviceBackendVulkan::DeviceBackendVulkan(DeviceBackendVulkan &&mov): context(mov.context) {
	mov.context.invalidate();
}
