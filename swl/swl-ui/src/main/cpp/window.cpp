//
// Created by Michael Ong on 31/3/20.
//
#include "window.hpp"

#include <type_traits>

using namespace std;
using namespace swl::cx;
using namespace swl::ui;


Window::Window(const std::string &window_name, const Rect &size) {
}

Window::Window() = default;


void Window::setName(const std::string &name) {

}

void Window::setFrame(const cx::Rect &frame) {

}


void Window::addEventFrame(Event<void, const Window&, cx::Rect> event) {
	event_frame.push_back(event);
}

void Window::addEventActiveState(Event<void, const Window&, State> event) {
	event_activateState.push_back(event);
}

void Window::addEventClose(Event<void, const Window &> event) {

}
