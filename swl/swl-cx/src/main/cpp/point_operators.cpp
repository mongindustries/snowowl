//
// Created by Michael Ong on 28/3/20.
//

#include <point.hpp>

#include <cmath>
#include <limits>

using namespace std;
using namespace swl::cx;

Point2D
	operator!    (const Point2D &pos) {
		return {
			-pos.x,
			-pos.y
		};
	}

Point2D
	operator+    (const Point2D &lhs, const Point2D &rhs) {
		return {
			lhs.x + rhs.x,
			lhs.y + rhs.y
		};
	}

bool
	operator==   (const Point2D &lhs, const Point2D &rhs) {

		constexpr float eps = numeric_limits<float>::epsilon();

		return
			abs(lhs.x - rhs.x) <= eps &&
			abs(lhs.y - rhs.y) <= eps;
	}

// 3d point operations

Point3D
	operator!    (const Point3D &pos) {
		return {
				-pos.x,
				-pos.y,
				-pos.z
		};
	}

Point3D
	operator+    (const Point3D &lhs, const Point3D &rhs) {
		return {
				lhs.x + rhs.x,
				lhs.y + rhs.y,
				lhs.z + rhs.z
		};
	}


bool
	operator==   (const Point3D &lhs, const Point3D &rhs) {

		constexpr float eps = numeric_limits<float>::epsilon();

		return
				abs(lhs.x - rhs.x) <= eps &&
				abs(lhs.y - rhs.y) <= eps &&
				abs(lhs.z - rhs.z) <= eps;
	}
