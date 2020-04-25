//
// Created by Michael Ong on 6/4/20.
//

#include "window_surface.hpp"
#include "swl_window_backend.hpp"

using namespace std;

SNOW_OWL_NAMESPACE(ui)

using namespace backend;

window_surface::window_surface  (const cx::exp::ptr_ref<ui::window> &window):
	window(window), surface_handle(window_backend::instance->surface(window.pointer())) {
}


cx::size_2d
			window_surface::get_size    () const {
	return window->get_size();
}

cx::exp::ptr_ref<window>
      window_surface::get_window  () const {
	return window;
}


bool  window_surface::operator<   (const window_surface& rhs) const {
	return window->handle < rhs.window->handle;
}

bool  window_surface::operator==  (const window_surface &rhs) const {
	return window->handle == rhs.window->handle;
}

SNOW_OWL_NAMESPACE_END
