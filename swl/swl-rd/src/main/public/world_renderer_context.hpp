//
// Created by Michael Ong on 24/4/20.
//
#pragma once

#include <vector>

#include <header.hpp>

#include "entity.hpp"

SNOW_OWL_NAMESPACE(rd)

struct world_renderer_context {

	std::vector<entity> const& entities;
};

SNOW_OWL_NAMESPACE_END
