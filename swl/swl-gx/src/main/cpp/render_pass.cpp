//
// Created by Michael Ong on 25/4/20.
//
#include "render_pass.hpp"

SNOW_OWL_NAMESPACE(gx)

template<typename c>
using ptr_ref = cx::exp::ptr_ref<c>;

render_pass::render_pass  () noexcept = default;

render_pass::render_pass  (context &context, render_block &render_block) {

}

render_pass::~render_pass () = default;


void
  render_pass::set_viewport           (const cx::size_2d &value) {

}

void
  render_pass::set_scissor            (const cx::rect &value) {

}


void
  render_pass::bind_buffer            (render_pass_stage_binding binding, unsigned int index, const ptr_ref<buffer<buffer_type::typeData>> &buffer) {

}

void
  render_pass::bind_fragment_texture  (unsigned index, const ptr_ref<buffer<buffer_type::typeTexture2d>> &texture) {

}

void
  render_pass::bind_fragment_texture  (unsigned index, const ptr_ref<buffer<buffer_type::typeTexture3d>> &texture) {

}

void
  render_pass::bind_fragment_sampler  (unsigned int index, const ptr_ref<sampler> &sampler) {

}


void
  render_pass::draw                   (const render_pass_draw_range &vertex_range) {

}

void
  render_pass::draw                   (const render_pass_draw_range &index_range, const ptr_ref<buffer<buffer_type::typeData>> &buffer) {

}

SNOW_OWL_NAMESPACE_END
