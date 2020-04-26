#include <iostream>
#include <sstream>

#include <game_loop.hpp>
#include <file_manager.hpp>
#include <application.hpp>

using namespace swl;
using namespace cx;
using namespace ui;

struct AppGameLoop: game_loop {

	AppGameLoop(): game_loop(60, 4) {
	}

	void create() override {
	}

	void preFrame() override {

//		if (window.is_sizing()) {
//			std::unique_lock<std::mutex> lock(window.loop_lock);
//			window.loop_wait.wait(lock);
//		}
	}

	void update(std::chrono::milliseconds delta) override {
	}

	void render(float offset) override {
	}
};

struct App: application {

	exp::ptr<window>       window;
	exp::ptr<AppGameLoop>  gameLoop;

	App(void* instance): application(instance), window(nullptr), gameLoop(nullptr) {
	}

	void on_create  () override {

		const auto pp = cx::file_manager::resourcePath;

		window = create_window("[SnowOwl:] App", rect{{100, 100},
		                                              {800, 480}});

		gameLoop = exp::ptr<AppGameLoop>{ new AppGameLoop() };
		gameLoop->open();

		window->event_on_resize.emplace_back([&](const ui::window&, const rect&) {
			if (window->is_sizing()) {
				gameLoop->frame();
			}
		});

		window->event_on_close.emplace_back([&](const ui::window&) {
			gameLoop->close();
		});
	}

	void on_destroy () override {
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

	return application::runApplication(App(instance));
}
