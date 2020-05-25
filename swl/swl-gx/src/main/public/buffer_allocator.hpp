//
// Created by Michael Ong on 26/4/20.
//
#pragma once

#include <header.hpp>
#include <ownership.hpp>
#include <point.hpp>

#include "context.hpp"
#include "buffer.hpp"

#include "pipeline.hpp"

SNOW_OWL_NAMESPACE(gx)

/**
 * A construct for buffer allocations.
 *
 * This object handles buffer instance allocations. A buffer allocator
 * contains information on where and how to create buffer instances.
 */
struct buffer_allocator { SWL_REFERENCEABLE(buffer_allocator) SWL_POLYMORPHIC(buffer_allocator)

  buffer_allocator    (gx::context &context, size_t initial_size);
  
  template < typename Type, uint64_t Size = 1 >
  cx::exp::ptr < buffer < pipeline::buffer_type::typeData > >
    create_data       (pipeline::buffer_visibility_type usage, size_t allocator_offset) {
      return create_data(usage, allocator_offset, sizeof(Type) * Size, sizeof(Type));
    }

  virtual cx::exp::ptr < buffer < pipeline::buffer_type::typeData > >
    create_data       (pipeline::buffer_visibility_type usage, size_t allocator_offset, size_t size, size_t stride);

  virtual cx::exp::ptr < buffer < pipeline::buffer_type::typeTexture2d > >
    create_texture2d  (pipeline::buffer_visibility_type usage, cx::size_2d const &dimension, pipeline::format format);

  virtual cx::exp::ptr < buffer < pipeline::buffer_type::typeTexture3d > >
    create_texture3d  (pipeline::buffer_visibility_type usage, cx::size_3d const &dimension, pipeline::format format);
};

SNOW_OWL_NAMESPACE_END
