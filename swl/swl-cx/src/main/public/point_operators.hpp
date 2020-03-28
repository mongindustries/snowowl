//
// Created by Michael Ong on 28/3/20.
//
#pragma once

#include "headerconv.hpp"
#include "point.hpp"

SNOW_OWL_NAMESPACE(cx)

// 2d point operations

Point2D
	operator!    (const Point2D &pos);

Point2D
	operator+    (const Point2D &lhs, const Point2D &rhs);

bool
	operator==   (const Point2D &lhs, const Point2D &rhs);

bool
	operator!=   (const Point2D &lhs, const Point2D &rhs) {
		return !(lhs == rhs);
	}

// 2d expansion

inline Point3D
	p3           (const Point2D &lhs) {
		return {
			lhs.x,
			lhs.y,
			0
		};
	}

// 3d point operations

Point3D
	operator+    (const Point3D &lhs, const Point3D &rhs);

bool
	operator==   (const Point3D &lhs, const Point3D &rhs);

bool
	operator!=   (const Point3D &lhs, const Point3D &rhs) {
		return !(lhs == rhs);
	}

// 3d trim

inline Point2D
	p2           (const Point3D &lhs) {
		return {
			lhs.x,
			lhs.y
		};
	}

SNOW_OWL_NAMESPACE_END
