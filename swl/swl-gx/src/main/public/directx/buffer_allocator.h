#pragma once

#include <header.hpp>

#include "directx/context.h"
#include "buffer_allocator.hpp"

SNOW_OWL_NAMESPACE(gx::dx)

struct buffer_allocator final : gx::buffer_allocator {

  buffer_allocator    (context &context, size_t initial_size);

  ~buffer_allocator   () override;

  cx::exp::ptr < buffer < pipeline::buffer_type::typeData > >
    create_data       (pipeline::buffer_visibility_type visbility, size_t allocator_offset, size_t size, size_t stride) override;

  cx::exp::ptr < buffer < pipeline::buffer_type::typeTexture2d > >
    create_texture2d  (pipeline::buffer_visibility_type visbility, cx::size_2d const &, pipeline::format) override;

  cx::exp::ptr < buffer < pipeline::buffer_type::typeTexture3d > >
    create_texture3d  (pipeline::buffer_visibility_type visbility, cx::size_3d const &, pipeline::format) override;


  winrt::com_ptr < ID3D12Heap > heap;
};

SNOW_OWL_NAMESPACE_END
