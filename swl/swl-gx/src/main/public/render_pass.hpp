//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#include <vector>

#include <header.hpp>
#include <rect.hpp>

#include "buffer.hpp"
#include "context.hpp"
#include "render_pipeline.hpp"
#include "resource_reference.hpp"
#include "transfer_block.hpp"

SNOW_OWL_NAMESPACE(gx)

struct render_block;

struct render_pass_resource_scope;

struct render_pass { SWL_REFERENCEABLE(render_pass) SWL_POLYMORPHIC(render_pass)

  enum topology_type {
    typeTriangleList,
    typeTriganleStrip,

    typeLineList,
    typeLineStrip,

    typePointList
  };

  render_pass       (render_block &render_block, std::array< pipeline::pass_output, NRS > const& output);


  virtual void
    topology        (topology_type type);


  virtual cx::exp::ptr < render_pass_resource_scope >
    buffer_prepare  (std::array< pipeline::pass_input, NBS > const &transitions);


  virtual void
    buffer_bind     (pipeline::shader_stage const &binding, int slot, cx::exp::ptr_ref < resource_reference > const &reference);

  virtual void
    buffer_bind     (pipeline::shader_stage const &binding, std::array < cx::exp::ptr_ref < resource_reference >, NBS > const &references);


  virtual void
    draw            (pipeline::draw_range const &vertex_range);
};

struct render_pass_resource_scope { SWL_BLOCK_CONTEXT(render_pass_resource_scope) SWL_POLYMORPHIC(render_pass_resource_scope)

  render_pass_resource_scope  (render_pass &pass, std::vector< pipeline::pass_input > const &input);
};

SNOW_OWL_NAMESPACE_END
