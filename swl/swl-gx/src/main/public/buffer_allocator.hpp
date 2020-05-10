//
// Created by Michael Ong on 26/4/20.
//
#pragma once

#include <header.hpp>
#include <ownership.hpp>
#include <point.hpp>

#include "context.hpp"
#include "buffer.hpp"

#include "render_pipeline.hpp"

SNOW_OWL_NAMESPACE(gx)

/// Specifies resource buffer usage for the allocator.
enum buffer_allocator_usage {
  /// Use for GPU only resources (textures). Memory resides in GPU. Buffer allocator will create
  /// another buffer for uploading updates from the CPU to the GPU.
  usagePrivate,
  /// Use for GPU reads and CPU writes (vertex buffers, index buffers). Memory resides in CPU and
  /// possibly GPU. Buffer allocator will synchronize updates between CPU/GPU. 
  usageShared
};

/**
 * A construct for buffer allocations.
 *
 * This object handles buffer instance allocations. A buffer allocator
 * contains information on where and how to create buffer instances.
 */
struct buffer_allocator { SWL_REFERENCEABLE(buffer_allocator) SWL_POLYMORPHIC(buffer_allocator)

  buffer_allocator    (gx::context& context);

  cx::exp::ptr<buffer<typeData>>
    create_data       (size_t size);

  cx::exp::ptr<buffer<typeTexture2d>>
    create_texture2d  (cx::size_2d const& dimension, pipeline::format format);

  cx::exp::ptr<buffer<typeTexture3d>>
    create_texture3d  (cx::size_3d const& dimension, pipeline::format format);
};

SNOW_OWL_NAMESPACE_END
