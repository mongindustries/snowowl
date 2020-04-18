#include <iostream>

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

	void update(std::chrono::milliseconds delta) override {
	}

	void render(float offset) override {

		auto& window = surface.getWindow().get();
		
		if (window.isSizing()) {

			std::unique_lock<std::mutex> lock(window.resizeMutex);
			window.resizeRender.wait(lock); // wait for window resize event to be finished

			renderer.frame();
		} else {
			renderer.frame();
		}
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
