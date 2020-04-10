//
// Created by micha on 4/3/2020.
//

#include <iostream>

#include "application.hpp"

#include "window.hpp"

#include "swl_window_backend.hpp"
#include "swl_win32_window.hpp"

#include <Windows.h>
#include <dwmapi.h>

using namespace std;
using namespace swl::cx;
using namespace swl::ui;

LRESULT CALLBACK win32_windowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {

	auto handlw = Application::windowFromNativeHandle(hwnd);

	switch (message) {
	case WM_CLOSE: {

		if (handlw == 0) {
			CloseWindow(hwnd);
			return 0;
		}

		auto &window = Application::windowWithHandle(handlw);
		backend::WindowBackend::backend->Close(window);
		return 0;
	}
	case WM_GETMINMAXINFO: {

		auto minmax = reinterpret_cast<MINMAXINFO*>(lparam);
		minmax->ptMinTrackSize = { 400, 400 };
		
		return 0;
	}
	case WM_ACTIVATE: {

		if (handlw == 0) {
			return DefWindowProc(hwnd, message, wparam, lparam);
		}

		const auto& window = Application::windowWithHandle(handlw);

		Window::State state;

		if (LOWORD(wparam) == WA_INACTIVE) {
			state = Window::State::Paused;
		} else {
			state = Window::State::Active;
		}

		if (HIWORD(wparam)) {
			state = Window::State::Background;
		}

		for (const auto& item : window._event_state_list) {
			item.invoke(window, state);
		}

		return 0;
	}
	case WM_WINDOWPOSCHANGED: {

		if (handlw == 0) {
			return DefWindowProc(hwnd, message, wparam, lparam);
		}

		auto &window = Application::windowWithHandle(handlw);

		RECT windowRect;
		GetClientRect(hwnd, &windowRect);

		backend::WindowBackend::backend->Resized(window, Rect{
			{ float(windowRect.left), float(windowRect.top) },
			{ float(windowRect.right - windowRect.left), float(windowRect.bottom - windowRect.top) } });
		return 0;
	}
	case WM_QUIT:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}
}

DriverHandle Application::windowFromNativeHandle(void* native_handle) {

	auto& nh = backend::WindowBackend::backend->activeNativeHandles;

	const auto result = find_if(nh.begin(), nh.end(), [native_handle](const pair<const reference_wrapper<Window>, void*> &item) -> bool {
		return static_cast<backend::win32_window*>(item.second)->hwnd == native_handle;
	});

	if (result == nh.end()) {
		return 0;
	}

	return get<0>(*result).get().getHandle();
}

Window& Application::windowWithHandle(DriverHandle handle) {

	auto& nh = backend::WindowBackend::backend->activeNativeHandles;

	const auto result = find_if(nh.begin(), nh.end(), [handle](const pair<const reference_wrapper<Window>, void*>& item) -> bool {
		return item.first.get().getHandle() == handle;
	});

	return get<0>(*result);
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
	customClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	customClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
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
