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

window* windowFromHWND(HWND hwnd) {

	for (const auto &item : backend::window_backend::instance->native_handles) {

		auto winItem = static_cast<backend::win32_window*>(get<1>(item));

		if (winItem->hwnd == hwnd) {
			return const_cast<window*>(winItem->reference);
		}
	}

	return nullptr;
}

void updateSizeLock(HWND hwnd, window* window) {

	RECT rect{};
	GetClientRect(hwnd, &rect);

	window->get_sink()->frame(::rect {
		{ static_cast<float>(rect.left), static_cast<float>(rect.top) },
		{ rect.right - rect.left, rect.bottom - rect.top }
	});
}

LRESULT CALLBACK win32_windowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {

	switch (message) {
	case WM_ENTERSIZEMOVE: {
		if (const auto window = windowFromHWND(hwnd)) {
			window->get_sink()->sizing(true);

			OutputDebugString(L"Resize start\n");
			
			updateSizeLock(hwnd, window);
		}
		return 0;
	}
	case WM_EXITSIZEMOVE: {
		if (const auto window = windowFromHWND(hwnd)) {

			OutputDebugString(L"Resize end\n");
			
			updateSizeLock(hwnd, window);

			window->get_sink()->sizing(false);
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
			window->get_sink()->closed();
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

void application::pre_heat(application &app) {

	HINSTANCE instance = (HINSTANCE)app.native_instance;
	
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
	customClass.hIconSm = LoadIcon(instance, MAKEINTRESOURCE(109));

	RegisterClassEx(&customClass);
}

int application::run_loop(application& app) {

	MSG msg { };
	int res = 0;

	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		res = DispatchMessage(&msg);
	}

	return res;
}
