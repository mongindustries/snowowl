//
// Created by Michael Ong on 26/4/20.
//
#include "buffer_allocator.hpp"
#include "buffer.hpp"

SNOW_OWL_NAMESPACE(gx)

  buffer_staging::buffer_staging() noexcept = default;

  buffer_staging::~buffer_staging() = default;


  buffer_allocator::buffer_allocator() noexcept = default;

  buffer_allocator::buffer_allocator(gx::context& context, size_t initial_size) { }

  buffer_allocator::buffer_allocator(buffer_allocator&&) noexcept { }

  buffer_allocator::~buffer_allocator() = default;


  cx::exp::ptr<buffer<typeData>>
    buffer_allocator::create_data(buffer_usage usage, buffer_view_type view_type, size_t allocator_offset, size_t size, size_t stride) { return cx::exp::ptr<buffer<typeData>>{nullptr}; }

  cx::exp::ptr<buffer<typeTexture2d>>
    buffer_allocator::create_texture2d(cx::size_2d const& dimension, pipeline::format format) { return cx::exp::ptr<buffer<typeTexture2d>>{nullptr}; }

  cx::exp::ptr<buffer<typeTexture3d>>
    buffer_allocator::create_texture3d(cx::size_3d const& dimension, pipeline::format format) { return cx::exp::ptr<buffer<typeTexture3d>>{nullptr}; }

SNOW_OWL_NAMESPACE_END
