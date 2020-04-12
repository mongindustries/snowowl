#include <application.hpp>

#include <game_loop.h>
#include <graphicsCanvas.hpp>
#include <vulkanGraphicsContext.hpp>

#include "renderer.hpp"

#include <iostream>

using namespace swl;
using namespace cx;
using namespace ui;

struct AppGameLoop: GameLoop {

	gx::implem::VulkanGraphicsContext& context;

	Own<app::Renderer> renderer;


	Window& window;

	WindowSurface surface;

	AppGameLoop(gx::implem::VulkanGraphicsContext& context, Window& window, const WindowSurface &surface): GameLoop(60, 4),
		context  (context),
		renderer (nullptr),

		window   (window),
		surface  (surface) {
	}

	void create() override {
		renderer = new app::Renderer(context, window, surface);
	}

	void update(std::chrono::milliseconds delta) override {
	}

	void render(float offset) override {
		renderer->frame();
	}
};

struct App: Application {

	Own<AppGameLoop> gameLoop;

	gx::GraphicsCanvas<gx::implem::VulkanGraphicsContext> canvas{ gx::implem::VulkanGraphicsContext() };

	App(void* instance): Application(instance), gameLoop(nullptr) {
	}

	void applicationCreate  () override {

		const auto window  = createWindow("[SnowOwl:] App", Rect{ { 100, 100 }, { 800, 480 } });
		auto       surface = window().getSurface();

		canvas.context().makeSurface(surface);

		gameLoop = new AppGameLoop(canvas.context(), window(), surface);
		gameLoop->open();
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
