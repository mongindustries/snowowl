//
// Created by micha on 4/3/2020.
//
#include "application.hpp"
#include "window.hpp"

using namespace std;

using namespace swl::cx;
using namespace swl::ui;

Application::Application(void *nativeInstance):
	nativeInstance(nativeInstance),
	windowList(),
	window("Hello", Rect { }) {

}
