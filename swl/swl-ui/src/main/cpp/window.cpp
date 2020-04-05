//
// Created by Michael Ong on 31/3/20.
//
#include "window.hpp"
#include "swl_window_backend.hpp"

#include <type_traits>
#include <utility>

using namespace std;
using namespace swl::cx;
using namespace swl::ui;
using namespace swl::ui::backend;

WindowBackend *WindowBackend::backend = new WindowBackend();


Window::Window() = default;

Window::Window(string window_name, const Rect &frame):
	_title       (std::move(window_name)),
	_frame       (frame) {

	WindowBackend::backend->Spawn(this);
}


void Window::setTitle(const std::string &new_title) {

	_title = new_title;
	WindowBackend::backend->UpdateTitle(this);
}

void Window::setFrame(const cx::Rect &new_frame) {

	_frame = new_frame;
	WindowBackend::backend->UpdateFrame(this);
}


windowSurface Window::getSurface() const {
	return WindowBackend::backend->Surface(this);
}


void Window::addEventFrame(const Event<void, const Window&, cx::Rect> &event) {
}

void Window::addEventActivate(const Event<void, const Window&, State> &event) {
}

void Window::addEventClose(const Event<void, const Window&> &event) {
}
