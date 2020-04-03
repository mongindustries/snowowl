//
// Created by micha on 3/29/2020.
//
#pragma once

#include <regex>

#include "path.hpp"
#include "headerconv.hpp"

SNOW_OWL_NAMESPACE(cx_private)

struct __PathComponents {

	std::string protocol;
	std::string path;

	std::string query;
};

struct PathOps {
	static __PathComponents components(const std::string &path, char separator);

	static std::regex match_path(const std::string &path, char separator);
};

SNOW_OWL_NAMESPACE_END
