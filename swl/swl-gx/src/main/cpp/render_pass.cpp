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
  render_pass::set_viewport   (const cx::size_2d &value) {

}

void
  render_pass::set_scissor    (const cx::rect &value) {

}

void
  render_pass::set_topology   () {
  
}


void
  render_pass::bind_sampler   (render_pass_stage_binding binding, int slot, cx::exp::ptr_ref<sampler> const& sampler) {
  
}


void
  render_pass::bind_samplers  (render_pass_stage_binding binding, std::vector<cx::exp::ptr_ref<sampler>> const& samplers) {
  
}


void
  render_pass::bind_buffer    (render_pass_stage_binding binding, int slot, cx::exp::ptr_ref<resource_reference> const& reference) {
  
}

void
  render_pass::bind_buffers   (render_pass_stage_binding binding, std::vector<cx::exp::ptr_ref<resource_reference>> const& references) {
  
}


void
  render_pass::draw           (const render_pass_draw_range &vertex_range) {

}

SNOW_OWL_NAMESPACE_END
