#pragma once

#include <header.hpp>

#include "directx/context.h"
#include "buffer_allocator.hpp"

SNOW_OWL_NAMESPACE(gx::dx)

struct buffer_allocator final : gx::buffer_allocator {

  buffer_allocator    (context &context, size_t initial_size);

  ~buffer_allocator   () override;
  
  template < typename Type, uint64_t Size = 1 >
  cx::exp::ptr < buffer < pipeline::buffer_type::typeData > >
    create_data       (pipeline::buffer_visibility_type usage, size_t allocator_offset) {
      return create_data(usage, allocator_offset, sizeof(Type) * Size, sizeof(Type));
    }

  cx::exp::ptr < buffer < pipeline::buffer_type::typeData > >
    create_data       (pipeline::buffer_visibility_type visibility, size_t allocator_offset, size_t size, size_t stride) override;

  cx::exp::ptr < buffer < pipeline::buffer_type::typeTexture2d > >
    create_texture2d  (pipeline::buffer_visibility_type visibility, cx::size_2d const &, pipeline::format) override;

  cx::exp::ptr < buffer < pipeline::buffer_type::typeTexture3d > >
    create_texture3d  (pipeline::buffer_visibility_type visibility, cx::size_3d const &, pipeline::format) override;


  winrt::com_ptr < ID3D12Heap > heap;
};

SNOW_OWL_NAMESPACE_END
