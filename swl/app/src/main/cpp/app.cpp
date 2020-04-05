#include <application.hpp>

using namespace swl::cx;
using namespace swl::ui;

struct App: public Application {

	App(void* instance): Application(instance) {
	}

	void applicationCreate  () override {

		auto &window = createWindow("Caption Title", Rect { { 0, 0 }, { 400, 400 } });
	}

	void applicationDestroy () override {
	}
};

int main () {
	return Application::runApplication(App(nullptr));
}
