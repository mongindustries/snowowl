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

inline swl::cx::rect scaled(swl::cx::rect const& input, const float scale) {
  return swl::cx::rect{
      swl::cx::point_2d(input.origin.x() * scale, input.origin.y() * scale),
      swl::cx::size_2d((int) ((float) input.size.x() * scale), (int) ((float) input.size.y() * scale))
  };
}
