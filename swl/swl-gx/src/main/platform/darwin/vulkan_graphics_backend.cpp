//
// Created by Michael Ong on 6/4/20.
//
#include "vulkan_context.hpp"
#include "vulkan_graphics_backend.hpp"

using namespace std;
using namespace swl::gx::backend;

VulkanGraphicsBackend* VulkanGraphicsBackend::instance = new VulkanGraphicsBackend();

vector<const char*> VulkanGraphicsBackend::vulkanExtensions = {
	VK_KHR_SURFACE_EXTENSION_NAME,
	VK_MVK_MACOS_SURFACE_EXTENSION_NAME
};

vector<const char*> VulkanGraphicsBackend::vulkanLayers = {

};

vk::UniqueSurfaceKHR
	VulkanGraphicsBackend::makeSurface(vk::Instance const &vk_instance, const ui::WindowSurface &surface) {

	vk::MacOSSurfaceCreateInfoMVK makeSurface({}, (void*) surface);
	return vk_instance.createMacOSSurfaceMVKUnique(makeSurface);
}
