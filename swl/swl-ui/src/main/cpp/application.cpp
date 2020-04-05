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

	vector<Own<Window>> windowList;
	
	WindowBackend::backend->application = this;
}

Application::Application(Application&& mov) noexcept:
	_native_instance(mov._native_instance),
	_window_list(move(mov._window_list)) {

	mov._native_instance = nullptr;
	mov._window_list.clear();
}

Application& Application::operator=(Application&& move) noexcept {

	_native_instance = move._native_instance;
	_window_list = std::move(move._window_list);

	move._native_instance = nullptr;
	move._window_list.clear();
	
	return *this;
}

MutableBorrow<Window> Application::createWindow(const std::string& caption, const Rect& frame) {
	return MutableBorrow { _window_list.emplace_back(caption, frame) };
}
