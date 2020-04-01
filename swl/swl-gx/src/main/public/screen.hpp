//
// Created by micha on 4/1/2020.
//
#pragma once

#include <headerconv.hpp>

#include "graphics.hpp"

SNOW_OWL_NAMESPACE(gx)

struct Screen {

	DriverHandle handle;

	int width;
	int height;

	float scale;

	short colorBitDepth[4];

	bool canHdr;

	friend class Device;

private:

	Device& device;
};

SNOW_OWL_NAMESPACE_END
