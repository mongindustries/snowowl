//
// Created by Michael Ong on 25/4/20.
//
#include "context.hpp"
#include "render_pipeline.hpp"

SNOW_OWL_NAMESPACE(gx)

pipeline::shader_stage::operator
  uint8_t     () const {
  return value;
}

pipeline::shader_stage pipeline::shader_stage::vertex   = shader_stage{ 0 };
pipeline::shader_stage pipeline::shader_stage::fragment = shader_stage{ 1 };


pipeline::write_mask::operator
  uint8_t     () const {
  return value;
}

bool pipeline::write_mask::operator&          (const write_mask& rhs) const {
  return value & rhs.value;
}

pipeline::write_mask pipeline::write_mask::r  = write_mask{ 0b0001 };
pipeline::write_mask pipeline::write_mask::g  = write_mask{ 0b0010 };
pipeline::write_mask pipeline::write_mask::b  = write_mask{ 0b0100 };
pipeline::write_mask pipeline::write_mask::a  = write_mask{ 0b1000 };


render_pipeline::render_pipeline  () noexcept = default;

render_pipeline::~render_pipeline () = default;


render_pipeline::render_pipeline  (context &) { }


render_pipeline::render_pipeline            (render_pipeline&& mov) noexcept
  : stages        (std::move(mov.stages))
  , raster        (std::move(mov.raster))
  , depth         (std::move(mov.depth))
  , stencil       (std::move(mov.stencil))
  , sample        (std::move(mov.sample))
  , topology_type (mov.topology_type) { }

render_pipeline& render_pipeline::operator= (render_pipeline&& mov) noexcept {

  stages        = std::move(mov.stages);
  raster        = std::move(mov.raster);
  depth         = std::move(mov.depth);
  stencil       = std::move(mov.stencil);
  sample        = std::move(mov.sample);
  topology_type = mov.topology_type;

  return *this;
}


void
  render_pipeline::construct  () { }

SNOW_OWL_NAMESPACE_END
