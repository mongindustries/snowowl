#include <application.hpp>

#include <graphicsCanvas.hpp>
#include <vulkanGraphicsContext.hpp>

#include "renderer.hpp"

using namespace swl;
using namespace cx;
using namespace ui;

struct App: Application {

	WindowSurface surface;

	gx::GraphicsCanvas<gx::implem::VulkanGraphicsContext> canvas{ gx::implem::VulkanGraphicsContext() };

	cx::Own<app::Renderer> renderer;


	App(void* instance): Application(instance), renderer(nullptr) {
	}

	void applicationCreate  () override {

		auto window = createWindow("[SnowOwl:] App",
			Rect { { 100, 100 }, { 800, 480 } });

		renderer = new app::Renderer(canvas.context(), window());
	}

	void applicationDestroy () override {
	}
};

#if defined(SWL_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR cmdArgs, int cmdShow) {
#elif defined(SWL_DARWIN)
int main () {
	std::nullptr_t instance = nullptr; // implied [NSApplication sharedApplication];
#endif

	return Application::runApplication(App(instance));
}
