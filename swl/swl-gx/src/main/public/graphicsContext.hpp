//
// Created by Michael Ong on 6/4/20.
//
#pragma once

#include <header.hpp>
#include <windowSurface.hpp>

#include "graphicsSwapChain.h"

SNOW_OWL_NAMESPACE(gx)

template<typename Context>
struct GraphicsContext {

	virtual ~GraphicsContext() = default;
};

SNOW_OWL_NAMESPACE_END
