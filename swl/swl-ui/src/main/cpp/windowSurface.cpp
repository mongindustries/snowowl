//
// Created by Michael Ong on 6/4/20.
//

#include "windowSurface.hpp"
#include "swl_window_backend.hpp"

using namespace std;

using namespace swl::ui;
using namespace swl::ui::backend;
using namespace swl::cx;

WindowSurface::WindowSurface(exp::ptr<Window> &window):
	_window                 (window),
	_native_surface_handle  (WindowBackend::backend->NativeSurface(window.pointer())) {

}


bool WindowSurface::operator<   (const WindowSurface& rhs) const {
	return _window.get()._handle  < rhs._window.get()._handle;
}

bool WindowSurface::operator==  (const WindowSurface &rhs) const {
	return _window.get()._handle == rhs._window.get()._handle;
}

size_2d WindowSurface::getSize() const {
	return _window.get().getSize();
}

exp::ptr_ref<Window> WindowSurface::getWindow() const {
	return _window;
}
