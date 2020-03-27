//
// Created by Michael Ong on 26/3/20.
//
#pragma once

#include <string>
#include "headerconv.hpp"

SNOW_OWL_NAMESPACE(cx)

struct Path {

	static char SeparatorStyle;

	std::string absolutePath;

	std::string extension;
	std::string lastComponent;

	explicit Path(std::string &absolutePath);

	explicit Path(std::string absolutePath);
};

SNOW_OWL_NAMESPACE_END
