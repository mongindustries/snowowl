//
// Created by micha on 4/3/2020.
//

#include "swl_win32_window.hpp"

using namespace std;
using namespace swl::ui;
using namespace swl::ui::backend;

win32_window::win32_window(HINSTANCE instance, const string &name) {

	resize_event = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	constexpr wchar_t* className = L"snowowl: window";

	// translate &name to wide string

	wstring w_name;
	auto    w_nameSize = MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, nullptr, 0);

	w_name.resize(w_nameSize);
	MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, w_name.data(), w_nameSize);

	RECT windowRect = { 0, 0, 400, 400 };
	AdjustWindowRectEx(&windowRect, WS_OVERLAPPEDWINDOW, false, 0);
	
	hwnd = CreateWindowEx(
		WS_EX_NOREDIRECTIONBITMAP,
		className,
		w_name.c_str(),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr,
		nullptr,
		instance,
		nullptr
	);
}
