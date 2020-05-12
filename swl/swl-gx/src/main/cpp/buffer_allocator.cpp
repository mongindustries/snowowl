//
// Created by Michael Ong on 26/4/20.
//
#include "buffer_allocator.hpp"
#include "buffer.hpp"

SNOW_OWL_NAMESPACE(gx)

buffer_allocator::buffer_allocator  () noexcept { }

buffer_allocator::buffer_allocator  (gx::context& context, buffer_allocator_usage usage, size_t initial_size) { }

buffer_allocator::buffer_allocator  (buffer_allocator&&) noexcept { }

buffer_allocator::~buffer_allocator () { }


cx::exp::ptr<buffer<typeData>>
  buffer_allocator::create_data(size_t size) {
  return cx::exp::ptr<buffer<typeData>>{ nullptr };
}

cx::exp::ptr<buffer<typeTexture2d>>
  buffer_allocator::create_texture2d(cx::size_2d const& dimension, pipeline::format format) {
  return cx::exp::ptr<buffer<typeTexture2d>>{ nullptr };
}

cx::exp::ptr<buffer<typeTexture3d>>
  buffer_allocator::create_texture3d(cx::size_3d const& dimension, pipeline::format format) {
  return cx::exp::ptr<buffer<typeTexture3d>>{ nullptr }; 
}




SNOW_OWL_NAMESPACE_END
