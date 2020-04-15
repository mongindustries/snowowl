//
// Created by Michael Ong on 4/4/20.
//
#pragma once

#include <header.hpp>

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


	Application* application{};

	std::map< Window const*, void* > activeNativeHandles;


	void Spawn            (Window const* window);


	void UpdateTitle      (Window const* window);

	void UpdateFrame      (Window const* window);


	void RemoveEntry      (Window const* window);


	void* NativeSurface   (Window const* window);


	friend struct ui::Application;
};

SNOW_OWL_NAMESPACE_END
