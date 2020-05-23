//
// Created by Michael Ong on 25/4/20.
//
#include "context.hpp"
#include "render_pipeline.hpp"

SNOW_OWL_NAMESPACE(gx)

pipeline::shader_stage::operator
  uint8_t() const {
  return value;
}

pipeline::write_mask::operator
  uint8_t() const {
  return value;
}

bool
  pipeline::write_mask::operator&                       (const pipeline::write_mask& rhs) const {
  return value & rhs.value;
}

pipeline::write_mask pipeline::write_mask::r            = pipeline::write_mask{ 0b0001 };
pipeline::write_mask pipeline::write_mask::g            = pipeline::write_mask{ 0b0010 };
pipeline::write_mask pipeline::write_mask::b            = pipeline::write_mask{ 0b0100 };
pipeline::write_mask pipeline::write_mask::a            = pipeline::write_mask{ 0b1000 };

pipeline::shader_stage pipeline::shader_stage::vertex   = pipeline::shader_stage{ 0 };
pipeline::shader_stage pipeline::shader_stage::fragment = pipeline::shader_stage{ 1 };


render_pipeline::render_pipeline  () noexcept { }

render_pipeline::render_pipeline  (render_pipeline&& mov) noexcept
  : shader_stages(std::move(mov.shader_stages))
  , raster(std::move(mov.raster))
  , depth(std::move(mov.depth))
  , stencil(std::move(mov.stencil))
  , sample(std::move(mov.sample))
  , topology_type(mov.topology_type)
  , independent_target_blend(mov.independent_target_blend)
  , render_inputs(std::move(mov.render_inputs))
  , render_outputs(std::move(mov.render_outputs)) { }

render_pipeline::~render_pipeline () { }


render_pipeline&
  render_pipeline::operator=      (render_pipeline&& mov) noexcept {

  shader_stages = std::move(mov.shader_stages);
  raster = std::move(mov.raster);
  depth = std::move(mov.depth);
  stencil = std::move(mov.stencil);
  sample = std::move(mov.sample);
  topology_type = mov.topology_type;
  independent_target_blend = mov.independent_target_blend;
  render_inputs = std::move(mov.render_inputs);
  render_outputs = std::move(mov.render_outputs);

  return *this;
}

void
  render_pipeline::construct      () { }

SNOW_OWL_NAMESPACE_END
