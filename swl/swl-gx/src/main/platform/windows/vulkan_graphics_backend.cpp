#include "vulkan_graphics_backend.hpp"
#include "vulkan_context.hpp"

using namespace std;
using namespace swl::gx::backend;

VulkanGraphicsBackend* VulkanGraphicsBackend::instance = new VulkanGraphicsBackend();

vector<const char*> VulkanGraphicsBackend::vulkanExtensions = {
	VK_KHR_SURFACE_EXTENSION_NAME,
	VK_KHR_WIN32_SURFACE_EXTENSION_NAME
};

vector<const char*> VulkanGraphicsBackend::vulkanLayers = {
};

vk::UniqueSurfaceKHR VulkanGraphicsBackend::makeSurface(const vk::Instance &vk_instance, const swl::ui::WindowSurface &surface) {
	return vk_instance.createWin32SurfaceKHRUnique(vk::Win32SurfaceCreateInfoKHR({ }, GetModuleHandle(nullptr), (HWND) (void*) surface));
}
