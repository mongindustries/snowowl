//
// Created by Michael Ong on 31/3/20.
//
#include "window.hpp"

#include "swl_window_sink.hpp"
#include "swl_window_backend.hpp"

#include <type_traits>
#include <utility>

using namespace std;
using namespace swl::cx;
using namespace swl::ui;
using namespace swl::ui::backend;

Window::Window() = default;

Window::Window(string window_name, const Rect &frame):
	_handle (cx::Core::makeHandle()),
	_title  (std::move(window_name)),
	_frame  (frame),
	_sink   (new WindowSink{ .handle = _handle }) {

	WindowBackend::backend->Spawn(this);
}

void
	Window::setTitle(const std::string &new_title) {

	_title = new_title;
	WindowBackend::backend->UpdateTitle(this);
}

void
	Window::setFrame(const cx::Rect &new_frame) {

	_frame = new_frame;
	WindowBackend::backend->UpdateFrame(this);
}


Size2D
	Window::getSize() const {
	return _frame.size;
}

Rect
	Window::getFrame() const {
	return _frame;
}

string
	Window::getTitle() const {
	return _title;
}

WindowSink*
	Window::getSink() const {
	return _sink;
}


bool Window::operator<  (const Window &rhs) const {
	return _handle < rhs._handle;
}

bool Window::operator== (const Window &rhs) const {
	return _handle == rhs._handle;
}
