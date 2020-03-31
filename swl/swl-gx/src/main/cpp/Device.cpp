//
// Created by micha on 3/31/2020.
//

#include "device.hpp"

#include "swl_vulkan_win32.hpp"

using namespace swl::gx;
using namespace std;

Device::Device() {

	const vk::ApplicationInfo appInfo(
			"snowowl: app",
			0x01'00'00,
			"snowowl:",
			0x01'00'00,
			VK_API_VERSION_1_0);

	const array<const char*, 1> enabled_extensions {
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME
	};

	vk::InstanceCreateInfo instanceInfo;

	instanceInfo.setPApplicationInfo(&appInfo);
	instanceInfo.setEnabledExtensionCount(enabled_extensions.size());
	instanceInfo.setPpEnabledExtensionNames(enabled_extensions.data());
}
