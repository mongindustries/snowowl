//
// Created by Michael Ong on 26/3/20.
//
#include <gtest/gtest.h>

#include "point.hpp"
#include "point_operators.hpp"

using namespace swl::cx;

TEST(Point2DTest, ShouldConstruct) {

	point_2d _2dPoint{100, 100 };

	ASSERT_EQ(_2dPoint.x(), 100);
	ASSERT_EQ(_2dPoint.y(), 100);
}

TEST(Point2DTest, ShouldAddPoint) {

	point_2d _2dPoint{100, 100 };

	point_2d _add = _2dPoint + _2dPoint;

	ASSERT_EQ(_add.x(), 200);
	ASSERT_EQ(_add.y(), 200);
}

TEST(Point3DTest, ShouldConstruct) {

	point_3d _3dPoint{100, 100, 100 };

	ASSERT_EQ(_3dPoint.x(), 100);
	ASSERT_EQ(_3dPoint.y(), 100);
	ASSERT_EQ(_3dPoint.z(), 100);
}
