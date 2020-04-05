#include "swl_window_backend.hpp"
#include "swl_win32_window.hpp"

#include <stdexcept>
#include <algorithm>

using namespace std;

using namespace swl::cx;

using namespace swl::ui;
using namespace backend;

void WindowBackend::Spawn(Window* window) {

	auto win32Win = new win32_window(static_cast<HINSTANCE>(application->_native_instance), window->_title);
	
	window->_handle = Core::makeHandle();
	win32Win->reference = window;

	SetWindowPos(win32Win->hwnd, nullptr, int(window->_frame.origin.x), int(window->_frame.origin.y), int(window->_frame.size.x), int(window->_frame.size.y), 0);

	ShowWindow(win32Win->hwnd, SW_SHOW | SW_SHOWNORMAL);
	UpdateWindow(win32Win->hwnd);

	activeNativeHandles.insert({window->_handle, win32Win });
}

void WindowBackend::UpdateTitle(Window const* window) {
	try {
		HWND handle = static_cast<win32_window*>(activeNativeHandles.at(window->_handle))->hwnd;

		wstring newTitle;
		const auto titleSize = MultiByteToWideChar(CP_UTF8, 0, window->_title.c_str(), -1, nullptr, 0);

		newTitle.resize(titleSize);
		MultiByteToWideChar(CP_UTF8, 0, window->_title.c_str(), -1, newTitle.data(), titleSize);

		SetWindowText(handle, newTitle.c_str());

	}
	catch (const out_of_range&) { }
}

void WindowBackend::UpdateFrame(Window const* window) {
	try {
		HWND handle = static_cast<win32_window*>(activeNativeHandles.at(window->_handle))->hwnd;

		auto& frame = window->_frame;
		SetWindowPos(handle, nullptr, frame.origin.x, frame.origin.y, frame.size.x, frame.size.y, 0);
	}
	catch (const out_of_range&) { }
}

void WindowBackend::Close(Window const* window) {
	try {
		auto win32Window = static_cast<win32_window*>(activeNativeHandles.at(window->_handle));

		for (const auto& item : window->_event_close_list) {
			item(*window);
		}

		DestroyWindow(win32Window->hwnd);

		delete win32Window;
		activeNativeHandles.erase(window->_handle);

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

void WindowBackend::Resized(Window* window, const Rect& rect) {
	window->_frame = rect;
}


windowSurface WindowBackend::Surface(const Window* window) {
	return windowSurface{ Core::makeHandle() };
}
