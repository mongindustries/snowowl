//
// Created by micha on 4/3/2020.
//

#include "swl_win32_window.hpp"

using namespace std;
using namespace swl::ui;
using namespace swl::ui::backend;

LRESULT CALLBACK win32_windowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {

	switch (message) {
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
	case WM_QUIT:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}
}

win32_window::win32_window(HINSTANCE instance, const string &name) {

	constexpr wchar_t* className = L"snowowl: window";
	WNDCLASSEX customClass { };

	customClass.lpfnWndProc = win32_windowProc;
	customClass.hInstance = instance;
	customClass.lpszClassName = className;

	RegisterClassEx(&customClass);

	// translate &name to wide string

	wstring w_name;
	auto    w_nameSize = MultiByteToWideChar(
		CP_UTF8,
		0,
		name.c_str(),
		-1,
		nullptr,
		0
	);

	w_name.resize(w_nameSize);
	MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, w_name.data(), w_nameSize);

	hwnd = CreateWindowEx(
		WS_EX_LAYERED,
		className,
		w_name.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800,
		480,
		nullptr,
		nullptr,
		instance,
		nullptr
	);
}
