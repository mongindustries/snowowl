//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#undef _USE_MATH_DEFINES

#include <array>

#include <header.hpp>

#include "context.hpp"
#include "pipeline.hpp"

SNOW_OWL_NAMESPACE(gx)

struct render_pipeline { SWL_REFERENCEABLE(render_pipeline) SWL_POLYMORPHIC(render_pipeline)

  explicit
    render_pipeline (context &context);

  virtual void
    construct       ();


  std::array < pipeline::pipeline_stage, 2 > 
                          stages        {};

  pipeline::raster        raster        {};
  pipeline::sample        sample        {};

  pipeline::depth         depth         {};
  pipeline::stencil       stencil       {};

  pipeline::topology_type topology_type {pipeline::topology_type::topologyTypeTriangle};
};

SNOW_OWL_NAMESPACE_END
