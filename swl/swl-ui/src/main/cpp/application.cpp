//
// Created by micha on 4/3/2020.
//
#include "application.hpp"

#include "window.hpp"
#include "swl_window_backend.hpp"

using namespace std;

using namespace swl::cx;
using namespace swl::ui;
using namespace swl::ui::backend;

Application::Application(void *nativeInstance):
	nativeInstance(nativeInstance),
	windowList() {

	WindowBackend::backend->application = this;
}

MutateOwn<Window> Application::createWindow(const string &caption, const Rect &frame) {
	return windowList.emplace_back(caption, frame);
}
