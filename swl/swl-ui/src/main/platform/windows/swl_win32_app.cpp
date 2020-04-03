//
// Created by micha on 4/3/2020.
//

#include "application.hpp"

#include "swl_win32_window.hpp"

using namespace std;
using namespace swl::cx;
using namespace swl::ui;

void Application::RunLoop(Application &app) {

	MSG msg { };

	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
