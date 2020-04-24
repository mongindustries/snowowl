//
// Created by Michael Ong on 4/4/20.
//

#include "window.hpp"
#include "application.hpp"

using namespace swl::ui;
using namespace swl::cx;

struct SampleApplication: public Application {
	explicit SampleApplication(void* instance) : Application(instance) {
	}

	void applicationCreate() override {

		auto &window = createWindow("Window", rect {100, 100, 800, 480 });
	}

	void applicationDestroy() override {
	}
};

int main() {

	Application::runApplication(SampleApplication(nullptr));

	return 0;
}
