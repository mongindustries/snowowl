//
// Created by Michael Ong on 26/3/20.
//
#pragma once

#include "headerconv.hpp"

SNOW_OWL_NAMESPACE(cx)

/**
 * A two coordinate location in space, z is always assumed zero.
 */
struct Point2D {
	float x, y;
};

/**
 * A three coordinate location in space.
 */
struct Point3D {
	float x, y, z;
};

typedef Point2D Size2D;

typedef Point3D Size3D;

SNOW_OWL_NAMESPACE_END
