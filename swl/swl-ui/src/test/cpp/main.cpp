//
// Created by micha on 4/3/2020.
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "window.hpp"
#include "application.hpp"

using namespace swl::ui;

struct SampleApp: public Application {

	explicit SampleApp(HINSTANCE instance): Application(instance) {
	}


	void applicationCreate() override {

	}

	void applicationDestroy() override {

	}
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow) {

	SampleApp app { instance };
	runApplication(app);
}
