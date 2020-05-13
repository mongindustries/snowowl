//
// Created by Michael Ong on 25/4/20.
//
#include "render_pass.hpp"

SNOW_OWL_NAMESPACE(gx)

template<typename c>
using ptr_ref = cx::exp::ptr_ref<c>;

render_pass::render_pass            () noexcept = default;

render_pass::render_pass            (context &context, render_block &render_block) {

}

render_pass::~render_pass           () = default;


render_pass::render_pass            (render_pass&&) noexcept {
  
}

render_pass& render_pass::operator= (render_pass&&) noexcept {
  return *this;
}


buffer_usage_block::buffer_usage_block  (cx::exp::ptr_ref<render_pass> const& pass, std::vector<std::pair<cx::exp::ptr_ref<resource_reference>, buffer_transition>> const& transitions):
  handle(pass->buffer_boundary(transitions)) { }

buffer_usage_block::~buffer_usage_block () {
  handle->release();
}


transition_handle::transition_handle    () = default;

transition_handle::~transition_handle   () = default;


void
  render_pass::set_viewport   (const cx::size_2d &value) {

}

void
  render_pass::set_scissor    (const cx::rect &value) {

}

void
  render_pass::set_topology   (topology_type type) {
}


cx::exp::ptr_ref<transition_handle>
  render_pass::buffer_boundary(std::vector<std::pair<cx::exp::ptr_ref<resource_reference>, buffer_transition>> const& transitions) {

  return cx::exp::ptr_ref{ nullptr };
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

void
  transition_handle::release  () {
  
}


SNOW_OWL_NAMESPACE_END
