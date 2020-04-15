//
// Created by Michael Ong on 12/4/20.
//
#pragma once

#include <header.hpp>
#include <windowSurface.hpp>

SNOW_OWL_NAMESPACE(gx)

template<typename Context>
struct GraphicsContext;

template<typename Context>
struct GraphicsSwapChain {

	explicit GraphicsSwapChain(const Context& context, const ui::WindowSurface& surface) { }

	virtual ~GraphicsSwapChain() = default;
};

SNOW_OWL_NAMESPACE_END
