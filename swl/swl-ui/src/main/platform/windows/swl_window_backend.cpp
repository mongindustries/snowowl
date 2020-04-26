#include "swl_window_backend.hpp"
#include "swl_win32_window.hpp"

#include <stdexcept>
#include <algorithm>

using namespace std;

using namespace swl::cx;

using namespace swl::ui;
using namespace backend;

window_backend* window_backend::instance = new window_backend();

void window_backend::create(window const* window) {

	auto win32Win = new win32_window(static_cast<HINSTANCE>(application->native_instance), window->get_title());
	win32Win->reference = window;

	auto frame = window->get_frame();
	RECT windowRect = { 0, 0, int(frame.size.x()), int(frame.size.y()) };
	AdjustWindowRectEx(&windowRect, WS_OVERLAPPEDWINDOW, false, 0);

	SetWindowPos(win32Win->hwnd, nullptr,
		int(frame.origin.x()), int(frame.origin.y()),
		int(windowRect.right - windowRect.left), int(windowRect.bottom - windowRect.top), SWP_FRAMECHANGED);

	ShowWindow(win32Win->hwnd, SW_SHOW | SW_SHOWNORMAL);
	UpdateWindow(win32Win->hwnd);

	native_handles.insert(pair { reference_wrapper(window), win32Win });
}

void window_backend::update_title(window const* window) {
	try {
		HWND handle = static_cast<win32_window*>(native_handles.at(window))->hwnd;

		wstring newTitle;
		const auto titleSize = MultiByteToWideChar(CP_UTF8, 0, window->get_title().c_str(), -1, nullptr, 0);

		newTitle.resize(titleSize);
		MultiByteToWideChar(CP_UTF8, 0, window->get_title().c_str(), -1, newTitle.data(), titleSize);

		SetWindowText(handle, newTitle.c_str());

	}
	catch (const out_of_range&) { }
}

void window_backend::update_frame(window const* window) {
	try {
		HWND handle = static_cast<win32_window*>(native_handles.at(window))->hwnd;

		auto frame = window->get_frame();
		RECT windowRect{};

		GetClientRect(handle, &windowRect);
		
		SetWindowPos(handle, nullptr, 
			int(frame.origin.x()), int(frame.origin.y()),
			int(windowRect.right - windowRect.left), int(windowRect.bottom - windowRect.top), SWP_FRAMECHANGED);
	}
	catch (const out_of_range&) { }
}

void window_backend::destroy(window const* window) {

	auto instance = static_cast<win32_window*>(native_handles.at(window));
	DestroyWindow(instance->hwnd);

	delete instance;
	native_handles.erase(window);

	if (native_handles.empty()) {
		delete instance;
		PostQuitMessage(0);
	}
}

void* window_backend::surface(const window *window) {
	return static_cast<win32_window*>(native_handles.at(window))->hwnd;
}
