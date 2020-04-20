#include <iostream>
#include <sstream>

#include <game_loop.hpp>
#include <file_manager.hpp>
#include <application.hpp>

#include <vulkan_context.hpp>

#include "renderer.hpp"

using namespace swl;
using namespace cx;
using namespace ui;

struct AppGameLoop: GameLoop {

	app::Renderer renderer;

	WindowSurface surface;

	AppGameLoop(const WindowSurface &surface): GameLoop(60, 4), renderer(surface), surface(surface) {
	}

	void create() override {
	}

	void preFrame() override {

		auto& window = surface.getWindow().get();

		if (window.isSizing()) {
			std::unique_lock<std::mutex> lock(window.lockForNoWindowResizing);
			window.waitForNoWindowResizing.wait(lock);
		}
	}

	void update(std::chrono::milliseconds delta) override {
		renderer.update();
	}

	void render(float offset) override {

		renderer.frame();
	}
};

struct App: Application {

	Own<Window>       window;
	Own<AppGameLoop>  gameLoop;

	App(void* instance): Application(instance), window(nullptr), gameLoop(nullptr) {
	}

	void applicationCreate  () override {

		const auto pp = cx::FileManager::resourcePath;

		window        = createWindow  ("[SnowOwl:] App", Rect{ { 100, 100 }, { 800, 480 } });
		auto surface  = WindowSurface (window);

		gameLoop = new AppGameLoop(surface);
		gameLoop->open();

		window->_event_size_list.emplace_back([&](const Window&, const Rect&) {
			if (window->isSizing()) {
				gameLoop->frame();
			}
		});

		window->_event_close_list.emplace_back([&](const Window&) {
			gameLoop->close();
		});
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
