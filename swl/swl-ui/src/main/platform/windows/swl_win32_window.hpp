//
// Created by micha on 4/3/2020.
//
#pragma once

#define UNICODE
#define WIN32_LEAN_AND_MEAN

#include <string>

#include <windows.h>
#include <headerconv.hpp>

SNOW_OWL_NAMESPACE(ui::backend)

struct win32_window {

	HWND hwnd;

	win32_window(HINSTANCE instance, const std::string &name);
};

SNOW_OWL_NAMESPACE_END
