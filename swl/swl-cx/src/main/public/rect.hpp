//
// Created by Michael Ong on 26/3/20.
//
#pragma once

#include "headerconv.hpp"
#include "point.hpp"

SNOW_OWL_NAMESPACE(cx)

struct SWL_EXPORT Rect {
	Point2D origin;
	Size2D size;
};

struct SWL_EXPORT Edge {
	float left, top, right, bottom;
};

SNOW_OWL_NAMESPACE_END
