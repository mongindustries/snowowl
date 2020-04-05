//
// Created by Michael Ong on 4/4/20.
//
#pragma once

#include <core.hpp>
#include <headerconv.hpp>

#include <map>

#include "window.hpp"
#include "windowSurface.hpp"
#include "application.hpp"

SNOW_OWL_NAMESPACE(ui)

struct Application;

SNOW_OWL_NAMESPACE_END

SNOW_OWL_NAMESPACE(ui::backend)

struct WindowBackend {

	static WindowBackend* backend;


	void Spawn(Window *window);

	void UpdateTitle(Window const* window);

	void UpdateFrame(Window const* window);


	void Close(Window const* window);

	void Resized(Window* window, const cx::Rect &rect);


	windowSurface Surface(const Window* window);


	friend struct ui::Application;

private:

	Application* application;

	std::map< cx::DriverHandle, void* > activeNativeHandles;
};

SNOW_OWL_NAMESPACE_END
