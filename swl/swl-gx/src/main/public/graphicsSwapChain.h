//
// Created by Michael Ong on 12/4/20.
//
#pragma once

#include <headerconv.hpp>
#include <windowSurface.hpp>

SNOW_OWL_NAMESPACE(gx)

template<typename Context>
struct GraphicsContext;

template<typename Context>
struct GraphicsSwapChain {

	struct Frame {
		uint16_t index;
	};


	explicit GraphicsSwapChain(const Context& context, const ui::WindowSurface& surface) { }

	virtual ~GraphicsSwapChain() = default;


	[[nodiscard]] virtual cx::Borrow<Frame> getFrame() = 0;
};

SNOW_OWL_NAMESPACE_END
