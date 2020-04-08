#include "vulkanGraphicsBackend.hpp"
#include "vulkanGraphicsContext.hpp"

using namespace std;
using namespace swl::gx::backend;

VulkanGraphicsBackend* VulkanGraphicsBackend::instance = new VulkanGraphicsBackend();

vector<const char*> VulkanGraphicsBackend::vulkanExtensions = {
	VK_KHR_SURFACE_EXTENSION_NAME,
	VK_KHR_WIN32_SURFACE_EXTENSION_NAME
};

vector<const char*> VulkanGraphicsBackend::vulkanLayers = {
};

void VulkanGraphicsBackend::makeSurface(vk::Instance const& instance, ui::WindowSurface& surface) {

	vk::Win32SurfaceCreateInfoKHR win32SurfaceCreate({},
		GetModuleHandle(nullptr),
		(HWND)surface.getNativeHandle()
	);

	surfaces.emplace(pair{ reference_wrapper{surface}, instance.createWin32SurfaceKHR(win32SurfaceCreate) });
}
