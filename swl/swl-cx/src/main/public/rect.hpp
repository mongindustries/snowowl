//
// Created by Michael Ong on 26/3/20.
//
#pragma once

#include "header.hpp"
#include "point.hpp"

SNOW_OWL_NAMESPACE(cx)

struct SWL_EXPORT rect {
	point_2d origin;
	size_2d  size;
};

struct SWL_EXPORT edge {
	float left, top, right, bottom;
};

SNOW_OWL_NAMESPACE_END
