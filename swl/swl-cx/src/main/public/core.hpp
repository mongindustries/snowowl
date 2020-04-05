//
// Created by Michael Ong on 26/3/20.
//
#pragma once

#include <string>
#include "headerconv.hpp"

SNOW_OWL_NAMESPACE(cx)

typedef unsigned long long DriverHandle;

struct SWL_EXPORT Core {
	static std::string version;

	static DriverHandle makeHandle();
};

SNOW_OWL_NAMESPACE_END
