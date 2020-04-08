#include "swl_window_backend.hpp"
#include "swl_win32_window.hpp"

#include <stdexcept>
#include <algorithm>

using namespace std;

using namespace swl::cx;

using namespace swl::ui;
using namespace swl::ui::backend;

void WindowBackend::Spawn(Window& window) {

	auto win32Win = new win32_window(static_cast<HINSTANCE>(application->_native_instance), window._title);
	
	window._handle = Core::makeHandle();
	win32Win->reference = &window;

	SetWindowPos(win32Win->hwnd, nullptr,
		int(window._frame.origin.x),
		int(window._frame.origin.y),
		int(window._frame.size.x),
		int(window._frame.size.y), 0);

	ShowWindow(win32Win->hwnd, SW_SHOW | SW_SHOWNORMAL);
	UpdateWindow(win32Win->hwnd);

	activeNativeHandles.insert(pair { reference_wrapper(window), win32Win });
}

void WindowBackend::UpdateTitle(Window const& window) {
	try {
		reference_wrapper<Window> ref_window { const_cast<Window&>(window) };
		HWND handle = static_cast<win32_window*>(activeNativeHandles.at(ref_window))->hwnd;

		wstring newTitle;
		const auto titleSize = MultiByteToWideChar(CP_UTF8, 0, window._title.c_str(), -1, nullptr, 0);

		newTitle.resize(titleSize);
		MultiByteToWideChar(CP_UTF8, 0, window._title.c_str(), -1, newTitle.data(), titleSize);

		SetWindowText(handle, newTitle.c_str());

	}
	catch (const out_of_range&) { }
}

void WindowBackend::UpdateFrame(Window const& window) {
	try {
		reference_wrapper<Window> ref_window{ const_cast<Window&>(window) };
		HWND handle = static_cast<win32_window*>(activeNativeHandles.at(ref_window))->hwnd;

		auto& frame = window._frame;
		SetWindowPos(handle, nullptr, frame.origin.x, frame.origin.y, frame.size.x, frame.size.y, 0);
	}
	catch (const out_of_range&) { }
}

void WindowBackend::Close(Window const& window) {
	try {

		reference_wrapper<Window> ref_window{ const_cast<Window&>(window) };
		auto handle = static_cast<win32_window*>(activeNativeHandles.at(ref_window));

		for (const auto& item : window._event_close_list) {
			item(window);
		}

		DestroyWindow(handle->hwnd);

		delete handle;
		activeNativeHandles.erase(ref_window);

		auto& lis = application->_window_list;
		const auto iter = find(lis.begin(), lis.end(), window);

		if (iter != lis.end()) {
			lis.erase(iter);
		}

		if (lis.empty()) {
			PostQuitMessage(0);
		}
	}
	catch (const out_of_range&) {}
}

void WindowBackend::Resized(Window& window, const Rect& rect) {
	window._frame = rect;
}

WindowSurface WindowBackend::PrepareSurface(const Window& window) {

	try {
		const reference_wrapper<Window> ref_window{ const_cast<Window&>(window) };
		const auto window_handle = static_cast<win32_window*>(activeNativeHandles.at(ref_window));

		WindowSurface newSurface;

		newSurface._handle = Core::makeHandle();
		newSurface._native_surface_handle = window_handle->hwnd;
		newSurface._window = &window;
		
		return newSurface;
	}
	catch (const out_of_range&) {
		return {};
	}
}
