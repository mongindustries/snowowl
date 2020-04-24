//
// Created by micha on 4/3/2020.
//
#include "application.hpp"

#include "swl_window_backend.hpp"
#include "window.hpp"

using namespace std;

using namespace swl::cx;
using namespace swl::ui;
using namespace swl::ui::backend;

Application::Application(void *native_instance):
	_native_instance(native_instance) {
	WindowBackend::backend->application = this;
}

exp::ptr<Window> Application::createWindow(const std::string& caption, const rect& frame) {
	return exp::ptr<Window>{ new Window(caption, frame) };
}
