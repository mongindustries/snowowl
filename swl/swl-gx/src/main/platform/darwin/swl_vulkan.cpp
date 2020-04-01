//
// Created by Michael Ong on 31/3/20.
//

#include "swl_vulkan.hpp"
#include "swl_vulkan_mac.hpp"

using namespace std;
using namespace swl::gx_private;

vector<const char *> VulkanPlatform::extensions_c_const() {
	return {
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_MVK_MACOS_SURFACE_EXTENSION_NAME
	};
}

std::vector<const char *> VulkanPlatform::debug_layers() {
	return {
	};
}

std::vector<const char *> VulkanPlatform::layers() {
	return {

	};
}
