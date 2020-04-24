//
// Created by Michael Ong on 26/3/20.
//
#pragma once

#include "header.hpp"

SNOW_OWL_NAMESPACE(cx)

typedef unsigned long long driver_handle;

struct SWL_EXPORT core {
	static const char* version;

	static driver_handle make_handle();
};

SNOW_OWL_NAMESPACE_END
