//
// Created by Michael Ong on 31/3/20.
//
#pragma once

#include <headerconv.hpp>

#include <string>
#include <vector>

SNOW_OWL_NAMESPACE(gx::internal)

struct SWL_EXPORT VulkanPlatform {

	static std::vector<const char*> extensions_c_const();

	static std::vector<const char*> debug_layers();

	static std::vector<const char*> layers();
};

SNOW_OWL_NAMESPACE_END
