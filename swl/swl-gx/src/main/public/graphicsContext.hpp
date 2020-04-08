//
// Created by Michael Ong on 6/4/20.
//
#pragma once

#include "windowSurface.hpp"
#include <headerconv.hpp>

SNOW_OWL_NAMESPACE(gx)

struct GraphicsContext {

	virtual ~GraphicsContext() { }


	virtual void makeSurface(ui::WindowSurface &surface) const = 0;



};

SNOW_OWL_NAMESPACE_END
