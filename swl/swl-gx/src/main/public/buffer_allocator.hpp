//
// Created by Michael Ong on 26/4/20.
//
#pragma once

#include <header.hpp>
#include <ownership.hpp>
#include <point.hpp>

#include "context.hpp"

SNOW_OWL_NAMESPACE(gx)

union buffer_size {

  cx::size_2d texture_size_2d;
  cx::size_3d texture_size_3d;

  size_t      byte_size;
};

/// Specifies resource buffer usage for the allocator.
enum buffer_allocator_usage {
  /// Use for GPU only resources (textures). Memory resides in GPU.
  usageDirect,
  /// Use for GPU reads and CPU writes (vertex buffers, index buffers). Memory resides in CPU and possibly GPU.
  usageShared
};

struct buffer_allocator { SWL_REFERENCEABLE(buffer_allocator) SWL_POLYMORPHIC(buffer_allocator)

  struct token {
    size_t position;
    size_t allocated_size;
  };
};

SNOW_OWL_NAMESPACE_END
