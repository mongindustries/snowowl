//
// Created by Michael Ong on 26/3/20.
//
#pragma once

#include "path.hpp"
#include "headerconv.hpp"

SNOW_OWL_NAMESPACE(cx)

Path MakePath(const std::string &abs) {
	return Path { .absolutePath = abs };
}

Path MakePathRelativeTo(const Path &source, const std::string &app) {
	return Path { .absolutePath = source.absolutePath + Path::SeparatorStyle + app };
}

SNOW_OWL_NAMESPACE_END
