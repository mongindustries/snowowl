//
// Created by micha on 4/3/2020.
//

#include <iostream>
#include <sstream>
#include <string>

#include "application.hpp"

#include "window.hpp"

#include "swl_window_sink.hpp"
#include "swl_window_backend.hpp"
#include "swl_win32_window.hpp"

#include <Windows.h>
#include <dwmapi.h>

using namespace std;
using namespace swl::cx;
using namespace swl::ui;

Window* windowFromHWND(HWND hwnd) {

	for (const auto &item : backend::WindowBackend::backend->activeNativeHandles) {

		auto winItem = static_cast<backend::win32_window*>(get<1>(item));

		if (winItem->hwnd == hwnd) {
			return const_cast<Window*>(winItem->reference);
		}
	}

	return nullptr;
}

void updateSizeLock(HWND hwnd, Window* window) {

	RECT rect{};
	GetClientRect(hwnd, &rect);

	window->getSink()->Update(Rect{ { float(rect.left), float(rect.top) }, { rect.right - rect.left, rect.bottom - rect.top } });
}

LRESULT CALLBACK win32_windowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {

	switch (message) {
	case WM_ENTERSIZEMOVE: {
		if (const auto window = windowFromHWND(hwnd)) {
			window->getSink()->Sizing(true);

			OutputDebugString(L"Resize start\n");
			
			updateSizeLock(hwnd, window);
		}
		return 0;
	}
	case WM_EXITSIZEMOVE: {
		if (const auto window = windowFromHWND(hwnd)) {

			OutputDebugString(L"Resize end\n");
			
			updateSizeLock(hwnd, window);

			window->getSink()->Sizing(false);
			window->waitForNoWindowResizing.notify_all();
		}
		return 0;
	}
	case WM_WINDOWPOSCHANGED: {

		if (const auto window = windowFromHWND(hwnd)) {

			OutputDebugString(L"Resizing...\n");
			updateSizeLock(hwnd, window);
		}

		return 0;
	}
	case WM_CLOSE: {
		if (const auto window = windowFromHWND(hwnd)) {
			window->getSink()->Closed();
		}
		return 0;
	}
	case WM_GETMINMAXINFO: {
		reinterpret_cast<MINMAXINFO*>(lparam)->ptMinTrackSize = { 400, 400 };
		return 0;
	}
	case WM_ACTIVATE: {
		return 0;
	}
	case WM_QUIT:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}
}

void Application::preHeat(Application &app) {

	HINSTANCE instance = (HINSTANCE)app._native_instance;
	
	constexpr wchar_t* className = L"snowowl: window";
	WNDCLASSEX customClass { };

	customClass.style = CS_HREDRAW | CS_VREDRAW;
	customClass.cbWndExtra = 0;
	customClass.cbClsExtra = 0;

	customClass.cbSize = sizeof(WNDCLASSEX);
	customClass.lpfnWndProc = win32_windowProc;
	customClass.hInstance = instance;
	customClass.lpszClassName = className;
	customClass.hIcon = LoadIcon(instance, MAKEINTRESOURCE(108));
	customClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	customClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	customClass.hIconSm = LoadIcon(instance, MAKEINTRESOURCE(108));

	RegisterClassEx(&customClass);
}

void Application::runLoop(Application &app) {

	MSG msg { };

	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
