//
// Created by micha on 4/1/2020.
//
#pragma once

#include <array>
#include <core.hpp>
#include <headerconv.hpp>

#include "graphics.hpp"

SNOW_OWL_NAMESPACE(gx)

struct SWL_EXPORT Context;

struct SWL_EXPORT Screen {

	cx::DriverHandle handle;

	int width;
	int height;

	float scale;

	std::array<short, 4> colorBitDepth;

	bool canHdr;

	Screen() = default;
};

SNOW_OWL_NAMESPACE_END
