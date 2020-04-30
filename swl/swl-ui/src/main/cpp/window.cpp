//
// Created by Michael Ong on 31/3/20.
//
#include <cassert>

#include "window.hpp"

#include "swl_window_sink.hpp"
#include "swl_window_backend.hpp"

#include <type_traits>
#include <utility>

using namespace std;
using namespace swl::cx;

SNOW_OWL_NAMESPACE(ui)

using namespace backend;

window::window            () = default;

window::window            (string window_name, const rect &frame):
	handle(cx::core::make_handle()), title(std::move(window_name)), frame(frame), sink(window_sink{ handle }) {
	window_backend::instance->create(this);
}

window::~window           () { }


window::window            (window &&mov) noexcept:
	handle(mov.handle), title(std::move(mov.title)), frame(mov.frame), sink(std::move(mov.sink)) {
}

window
			&window::operator=  (window &&) noexcept {
	return *this;
}


void  window::bind_loop   (const cx::exp::ptr_ref<cx::game_loop>& loop) {

	assert(!game_loop);

	game_loop = loop;
}


void  window::set_title   (const std::string &new_title) {

	title = new_title;
	window_backend::instance->update_title(this);
}

void  window::set_frame   (const cx::rect &new_frame) {

	frame = new_frame;
	window_backend::instance->update_frame(this);
}


size_2d
			window::get_size    () const {
	return frame.size;
}

rect  window::get_frame   () const {
	return frame;
}

string
			window::get_title   () const {
	return title;
}

exp::ptr_ref<window_sink>
			window::get_sink    () const {
	return exp::ptr_ref<window_sink>{ sink };
}

bool  window::is_sizing   () const {
	return resizing;
}


bool  window::operator<   (const window &rhs) const {
	return handle < rhs.handle;
}

bool  window::operator==  (const window &rhs) const {
	return handle == rhs.handle;
}

SNOW_OWL_NAMESPACE_END
