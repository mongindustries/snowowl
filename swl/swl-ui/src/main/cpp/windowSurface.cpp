//
// Created by Michael Ong on 6/4/20.
//
#include <functional>
#include "windowSurface.hpp"

using namespace swl::ui;


WindowSurface::WindowSurface(WindowSurface &&mov) noexcept :
	_window(mov._window),
	_native_surface_handle(mov._native_surface_handle) {
}

WindowSurface& WindowSurface::operator=(WindowSurface &&mov)  noexcept {
	_native_surface_handle = mov._native_surface_handle;
	_window = mov._window;

	mov._native_surface_handle = nullptr;
	mov._window = nullptr;

	return *this;
}


WindowSurface::~WindowSurface() {
	_native_surface_handle = nullptr;
	_window = nullptr;
}


void WindowSurface::presentAndWait() {

}


bool WindowSurface::operator<   (const WindowSurface& rhs) const {
	return _handle < rhs._handle;
}

bool WindowSurface::operator==  (const WindowSurface &rhs) const {
	return _handle == rhs._handle;
}


bool operator<  (const std::reference_wrapper<WindowSurface> lhr, const std::reference_wrapper<WindowSurface> rhs) {
	return lhr.get().getHandle() < rhs.get().getHandle();
}

bool operator== (const std::reference_wrapper<WindowSurface> lhr, const std::reference_wrapper<WindowSurface> rhs) {
	return lhr.get().getHandle() == rhs.get().getHandle();
}
