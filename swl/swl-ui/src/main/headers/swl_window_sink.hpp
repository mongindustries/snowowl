//
// Created by Michael Ong on 12/4/20.
//
#pragma once

#include <header.hpp>
#include <core.hpp>
#include <rect.hpp>

SNOW_OWL_NAMESPACE(ui)

struct window_sink {

	cx::driver_handle handle;


	void sizing   (bool value);

	void frame    (const cx::rect &new_rect);


	void closed   ();
};

SNOW_OWL_NAMESPACE_END
