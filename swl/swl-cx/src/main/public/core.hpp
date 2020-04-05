//
// Created by Michael Ong on 26/3/20.
//
#pragma once

#include <string>
#include "headerconv.hpp"

SNOW_OWL_NAMESPACE(cx)
struct SWL_EXPORT Core {
	static std::string version;

	static unsigned long makeHandle();
};

typedef unsigned long DriverHandle;


SNOW_OWL_NAMESPACE_END
