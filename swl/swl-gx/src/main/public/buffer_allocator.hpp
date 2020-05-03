//
// Created by Michael Ong on 26/4/20.
//
#pragma once

#include <header.hpp>
#include <ownership.hpp>
#include "context.hpp"
#include "buffer.hpp"

SNOW_OWL_NAMESPACE(gx)

struct buffer_allocator {
  buffer_allocator(context& context);
};

SNOW_OWL_NAMESPACE_END
