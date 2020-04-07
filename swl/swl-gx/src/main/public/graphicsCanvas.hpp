//
// Created by Michael Ong on 6/4/20.
//
#pragma once

#include <headerconv.hpp>
#include "graphicsContext.hpp"

SNOW_OWL_NAMESPACE(gx)

template<typename Context, std::enable_if_t< std::is_base_of_v<GraphicsContext, Context>, int> = 0xBEEF>
struct GraphicsCanvas {

	Context& context() {
		return implementation;
	}


	GraphicsCanvas(Context context): implementation(std::move(context)) {
	}

private:

	Context implementation;
};

SNOW_OWL_NAMESPACE_END
