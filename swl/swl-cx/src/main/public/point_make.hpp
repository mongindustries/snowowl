//
// Created by Michael Ong on 26/3/20.
//
#pragma once

#include "point.hpp"
#include "headerconv.hpp"

SNOW_OWL_NAMESPACE(cx)

Point3D MakePointFrom(const Point2D &_2d) {
	return Point3D { .x = _2d.x, .y = _2d.y, .z = 0 };
}

SNOW_OWL_NAMESPACE_END
