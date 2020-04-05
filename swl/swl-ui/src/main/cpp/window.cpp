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
	title       (std::move(window_name)),
	frame       (frame) {

	WindowBackend::backend->Spawn(this);
}


void Window::setTitle(const std::string &new_title) {

	title = new_title;
	WindowBackend::backend->UpdateTitle(this);
}

void Window::setFrame(const cx::Rect &new_frame) {

	frame = new_frame;
	WindowBackend::backend->UpdateFrame(this);
}


windowSurface Window::getSurface() const {
	return WindowBackend::backend->Surface(this);
}


void Window::addEventFrame(const Event<void, const Window&, cx::Rect> &event) {
	event_frame.push_back(event);
}

void Window::addEventActivate(const Event<void, const Window&, State> &event) {
	event_activateState.push_back(event);
}

void Window::addEventClose(const Event<void, const Window&> &event) {
	event_closeList.push_back(event);
}
