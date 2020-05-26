//
// Created by Michael Ong on 25/4/20.
//
#include "render_pass.hpp"

SNOW_OWL_NAMESPACE(gx)

template <typename c>
using ptr_ref = cx::exp::ptr_ref<c>;

render_pass::render_pass              (render_pass&&) noexcept { }

render_pass& render_pass::operator=   (render_pass&&) noexcept { return *this; }


render_pass::render_pass  () noexcept = default;

render_pass::~render_pass () = default;


render_pass::render_pass  (render_block& render_block, std::array< pipeline::pass_output, NRS > const& output) { }


void
  render_pass::topology       (topology_type type) {}


cx::exp::ptr<render_pass_resource_scope>
  render_pass::buffer_prepare (std::array< pipeline::pass_input, NBS > const& transitions) { return cx::exp::ptr<render_pass_resource_scope>{nullptr}; }


void
  render_pass::buffer_bind    (pipeline::shader_stage const &binding, int slot, cx::exp::ptr_ref<resource_reference> const& reference) { }

void
  render_pass::buffer_bind    (pipeline::shader_stage const &binding, std::array<cx::exp::ptr_ref<resource_reference>, NBS> const& references) { }


void
  render_pass::draw           (pipeline::draw_range const &vertex_range) {}


render_pass_resource_scope::render_pass_resource_scope  () noexcept = default;

render_pass_resource_scope::~render_pass_resource_scope () = default;


render_pass_resource_scope::render_pass_resource_scope  (render_pass &pass, std::vector < pipeline::pass_input > const &input) { }

SNOW_OWL_NAMESPACE_END
