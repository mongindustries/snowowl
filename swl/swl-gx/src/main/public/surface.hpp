//
// Created by Michael Ong on 31/3/20.
//
#pragma once

#include <core.hpp>
#include <headerconv.hpp>

#include <data.hpp>
#include <window.hpp>

#include "graphics.hpp"

SNOW_OWL_NAMESPACE(gx)

struct SWL_EXPORT Surface {

	Surface();

	cx::DriverHandle getHandle() const { return handle; }

private:

	cx::DriverHandle handle;
};

SNOW_OWL_NAMESPACE_END
