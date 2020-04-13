//
// Created by Michael Ong on 12/4/20.
//
#pragma once

#include <headerconv.hpp>
#include <core.hpp>
#include <rect.hpp>

SNOW_OWL_NAMESPACE(ui)

struct WindowSink {

	cx::DriverHandle handle;


	void Sizing(bool value);
	
	void Update(const cx::Rect &new_rect);

	void Closed();
};

SNOW_OWL_NAMESPACE_END
