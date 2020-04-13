#include "swl_window_backend.hpp"
#include "swl_win32_window.hpp"

#include <stdexcept>
#include <algorithm>

using namespace std;

using namespace swl::cx;

using namespace swl::ui;
using namespace swl::ui::backend;

WindowBackend* WindowBackend::backend = new WindowBackend();

void WindowBackend::Spawn(Window const* window) {

	auto win32Win = new win32_window(static_cast<HINSTANCE>(application->_native_instance), window->getTitle());
	win32Win->reference = window;

	auto frame = window->getFrame();
	RECT windowRect = { 0, 0, int(frame.size.x()), int(frame.size.y()) };
	AdjustWindowRectEx(&windowRect, WS_OVERLAPPEDWINDOW, false, 0);

	SetWindowPos(win32Win->hwnd, nullptr,
		int(frame.origin.x()), int(frame.origin.y()),
		int(windowRect.right - windowRect.left), int(windowRect.bottom - windowRect.top), SWP_FRAMECHANGED);

	ShowWindow(win32Win->hwnd, SW_SHOW | SW_SHOWNORMAL);
	UpdateWindow(win32Win->hwnd);

	activeNativeHandles.insert(pair { reference_wrapper(window), win32Win });
}

void WindowBackend::UpdateTitle(Window const* window) {
	try {
		HWND handle = static_cast<win32_window*>(activeNativeHandles.at(window))->hwnd;

		wstring newTitle;
		const auto titleSize = MultiByteToWideChar(CP_UTF8, 0, window->getTitle().c_str(), -1, nullptr, 0);

		newTitle.resize(titleSize);
		MultiByteToWideChar(CP_UTF8, 0, window->getTitle().c_str(), -1, newTitle.data(), titleSize);

		SetWindowText(handle, newTitle.c_str());

	}
	catch (const out_of_range&) { }
}

void WindowBackend::UpdateFrame(Window const* window) {
	try {
		HWND handle = static_cast<win32_window*>(activeNativeHandles.at(window))->hwnd;

		auto frame = window->getFrame();
		RECT windowRect = { 0, 0, int(frame.size.x()), int(frame.size.y()) };
		AdjustWindowRectEx(&windowRect, WS_OVERLAPPEDWINDOW, false, 0);

		SetWindowPos(handle, nullptr, 
			int(frame.origin.x()), int(frame.origin.y()),
			int(windowRect.right - windowRect.left), int(windowRect.bottom - windowRect.top), SWP_FRAMECHANGED);
	}
	catch (const out_of_range&) { }
}

void WindowBackend::RemoveEntry(Window const* window) {

	auto instance = reinterpret_cast<win32_window*>(activeNativeHandles.at(window));
	DestroyWindow(instance->hwnd);

	delete instance;
	activeNativeHandles.erase(window);

	if (activeNativeHandles.empty()) {
		delete backend;
		PostQuitMessage(0);
	}
}

void* WindowBackend::NativeSurface(const Window *window) {
	return static_cast<win32_window*>(activeNativeHandles.at(window))->hwnd;
}
