#include <application.hpp>

#include <vulkanGraphicsContext.hpp>
#include <game_loop.h>

#include "renderer.hpp"

using namespace swl;
using namespace cx;
using namespace ui;

struct AppGameLoop: GameLoop {

	app::Renderer renderer;

	AppGameLoop(const WindowSurface &surface): GameLoop(60, 4), renderer(surface) {
	}

	void create() override {
	}

	void update(std::chrono::milliseconds delta) override {
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

		window        = createWindow  ("[SnowOwl:] App", Rect{ { 100, 100 }, { 800, 480 } });
		auto surface  = WindowSurface (window);

		gameLoop = new AppGameLoop(surface);
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
