//
// Created by Michael Ong on 4/4/20.
//
#pragma once

#include <header.hpp>

#include <map>

#include "window.hpp"
#include "window_surface.hpp"

#include "application.hpp"

SNOW_OWL_NAMESPACE(ui)

struct application;

SNOW_OWL_NAMESPACE_END

SNOW_OWL_NAMESPACE(ui::backend)

struct window_backend {

	static window_backend* instance;


	application*                        application{};

	std::map< window const*, void* >    native_handles;


	void  create        (window const* window);

	void  destroy       (window const* window);


	void  update_title  (window const* window);

	void  update_frame  (window const* window);


	void* surface       (window const* window);

	friend struct ui::application;
};

SNOW_OWL_NAMESPACE_END
