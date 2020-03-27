//
// Created by Michael Ong on 26/3/20.
//
#pragma once

#include "rect.hpp"
#include "headerconv.hpp"

SNOW_OWL_NAMESPACE(cx)

Edge MakeEdge(float top, float sides, float bottom) {
	return Edge { .left = sides, .top = top, .right = sides, .bottom = bottom };
}

Edge MakeEdge(float horizontal, float vertical) {
	return Edge { .left = horizontal, .top = vertical, .right = horizontal, .bottom = vertical };
}

SNOW_OWL_NAMESPACE_END
