//
// Created by Michael Ong on 4/4/20.
//

#include "window.hpp"
#include "application.hpp"

using namespace swl::ui;
using namespace swl::cx;

struct SampleApplication: public application {
	explicit SampleApplication(void* instance) : application(instance) {
	}

	void on_create() override {

		auto &window = create_window("window", rect{100, 100, 800, 480});
	}

	void on_destroy() override {
	}
};

int main() {

	application::runApplication(SampleApplication(nullptr));

	return 0;
}
