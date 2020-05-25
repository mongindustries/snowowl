//
// Created by Michael Ong on 26/4/20.
//
#include "buffer_allocator.hpp"
#include "buffer.hpp"

SNOW_OWL_NAMESPACE(gx)

buffer_allocator::buffer_allocator  () noexcept = default;

buffer_allocator::~buffer_allocator () = default;


buffer_allocator::buffer_allocator  (gx::context& context, size_t initial_size) { }


buffer_allocator::buffer_allocator            (buffer_allocator&&) noexcept { }

buffer_allocator& buffer_allocator::operator= (buffer_allocator&&) noexcept { }


cx::exp::ptr<buffer<pipeline::buffer_type::typeData>>
  buffer_allocator::create_data       (pipeline::buffer_visibility_type visiblity, size_t allocator_offset, size_t size, size_t stride) {
    return cx::exp::ptr<buffer<pipeline::buffer_type::typeData>>{nullptr};
  }

cx::exp::ptr<buffer<pipeline::buffer_type::typeTexture2d>>
  buffer_allocator::create_texture2d  (pipeline::buffer_visibility_type visiblity, cx::size_2d const& dimension, pipeline::format format) {
    return cx::exp::ptr<buffer<pipeline::buffer_type::typeTexture2d>>{nullptr};
  }

cx::exp::ptr<buffer<pipeline::buffer_type::typeTexture3d>>
  buffer_allocator::create_texture3d  (pipeline::buffer_visibility_type visiblity, cx::size_3d const& dimension, pipeline::format format) {
    return cx::exp::ptr<buffer<pipeline::buffer_type::typeTexture3d>>{nullptr};
  }

SNOW_OWL_NAMESPACE_END
