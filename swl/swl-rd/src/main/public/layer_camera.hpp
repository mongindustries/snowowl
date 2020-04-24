//
// Created by Michael Ong on 22/4/20.
//
#pragma once

#include <header.hpp>
#include <point.hpp>

SNOW_OWL_NAMESPACE(rd)

struct layer_camera {

	cx::point_3d position;
	cx::point_3d rotation;
	cx::point_3d scale;
};

SNOW_OWL_NAMESPACE_END
