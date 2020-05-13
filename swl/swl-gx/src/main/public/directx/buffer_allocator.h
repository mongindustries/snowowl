#pragma once

#include <header.hpp>

#include "directx/context.h"
#include "buffer_allocator.hpp"

SNOW_OWL_NAMESPACE(gx::dx)

struct buffer_allocator final : gx::buffer_allocator {

  buffer_allocator    (dx::context& context, buffer_allocator_usage usage, size_t initial_size);

  ~buffer_allocator   () override;

  cx::exp::ptr<buffer<typeData>>
    create_data       (size_t size) override;

  cx::exp::ptr<buffer<typeTexture2d>>
    create_texture2d  (cx::size_2d const& dimension, pipeline::format format) override;

  cx::exp::ptr<buffer<typeTexture3d>>
    create_texture3d  (cx::size_3d const& dimension, pipeline::format format) override;


  winrt::com_ptr<ID3D12Heap> heap;
};

SNOW_OWL_NAMESPACE_END
