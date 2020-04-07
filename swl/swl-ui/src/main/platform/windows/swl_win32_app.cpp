//
// Created by micha on 4/3/2020.
//

#include "application.hpp"

#include "swl_window_backend.hpp"
#include "swl_win32_window.hpp"

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

		auto window = Application::windowWithHandle(handlw);
		backend::WindowBackend::backend->Close(&window);
		return 0;
	}
	case WM_GETMINMAXINFO: {

		auto minmax = reinterpret_cast<MINMAXINFO*>(lparam);
		minmax->ptMinTrackSize = { 400, 400 };
		
		return 0;
	}
	case WM_WINDOWPOSCHANGED: {

		if (handlw == 0) {
			return DefWindowProc(hwnd, message, wparam, lparam);
		}

		auto window = Application::windowWithHandle(handlw);
		const auto info = reinterpret_cast<WINDOWPOS*>(lparam);
		backend::WindowBackend::backend->Resized(&window, Rect{
			{ (float)info->x, (float)info->y },
			{ (float)info->cx, (float)info->cy } });
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

	auto& handles = backend::WindowBackend::backend->activeNativeHandles;
	auto result = find_if(handles.begin(), handles.end(), [native_handle](const pair<DriverHandle, void*> &item) {
		return static_cast<backend::win32_window*>(get<1>(item))->hwnd == static_cast<HWND>(native_handle);
	});

	if (result != handles.end()) {
		return result->first;
	}

	return 0;
}

Window& Application::windowWithHandle(cx::DriverHandle handle) {
	const auto item = static_cast<backend::win32_window*>(backend::WindowBackend::backend->activeNativeHandles.at(handle));
	return *item->reference;
}


void Application::preHeat(Application &app) {

	constexpr wchar_t* className = L"snowowl: window";
	WNDCLASSEX customClass { };

	customClass.style = CS_HREDRAW | CS_VREDRAW;
	customClass.cbWndExtra = 0;
	customClass.cbClsExtra = 0;

	customClass.cbSize = sizeof(WNDCLASSEX);
	customClass.lpfnWndProc = win32_windowProc;
	customClass.hInstance = (HINSTANCE) app._native_instance;
	customClass.lpszClassName = className;
	customClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	customClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	customClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	customClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	RegisterClassEx(&customClass);
}

void Application::runLoop(Application &app) {

	MSG msg { };

	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}