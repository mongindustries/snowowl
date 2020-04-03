//
// Created by Michael Ong on 31/3/20.
//

#include "swl_vulkan.hpp"
#include "swl_vulkan_win32.hpp"

using namespace std;
using namespace swl::gx::internal;

vector<const char *> VulkanPlatform::extensions_c_const() {
	return {
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME
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
