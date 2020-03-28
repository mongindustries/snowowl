//
// Created by Michael Ong on 26/3/20.
//
#include <gtest/gtest.h>

#include "point.hpp"

using namespace swl::cx;

TEST(Point2DTest, ShouldConstruct) {

	Point2D _2dPoint = { .x = 100, .y = 100 };

	ASSERT_EQ(_2dPoint.x, 100);
	ASSERT_EQ(_2dPoint.y, 100);
}

TEST(Point3DTest, ShouldConstruct) {

	Point3D _3dPoint = { .x = 100, .y = 100, .z = 100 };

	ASSERT_EQ(_3dPoint.x, 100);
	ASSERT_EQ(_3dPoint.y, 100);
	ASSERT_EQ(_3dPoint.z, 100);
}
