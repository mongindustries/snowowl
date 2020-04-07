//
// Created by Michael Ong on 6/4/20.
//
#include "vulkanGraphicsContext.hpp"
#include "vulkanGraphicsBackend.hpp"

using namespace std;
using namespace swl::gx::backend;

VulkanGraphicsBackend* VulkanGraphicsBackend::instance = new VulkanGraphicsBackend();

vector<const char*> VulkanGraphicsBackend::vulkanExtensions = {
	VK_KHR_SURFACE_EXTENSION_NAME,
	VK_MVK_MACOS_SURFACE_EXTENSION_NAME
};

void VulkanGraphicsBackend::makeSurface(vk::Instance &instance, ui::WindowSurface &surface) {

	vk::MacOSSurfaceCreateInfoMVK createInfo({ }, surface.getNativeHandle());
	surfaces.emplace(pair { reference_wrapper(surface), instance.createMacOSSurfaceMVK(createInfo) });
}
