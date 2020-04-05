#include <application.hpp>

using namespace swl::cx;
using namespace swl::ui;

struct App: Application {

	App(void* instance): Application(instance) {
	}

	void applicationCreate  () override {

		auto window = createWindow("Caption Title", Rect { { 100, 100 }, { 800, 480 } });
	}

	void applicationDestroy () override {
	}
};

#if defined(SWL_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR cmdArgs, int cmdShow) {
	return Application::runApplication(App(instance));
}

#elif defined(SWL_DARWIN)

int main () {
	return Application::runApplication(App(nullptr));
}

#endif
