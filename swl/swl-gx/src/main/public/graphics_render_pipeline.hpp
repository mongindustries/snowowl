//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#include <array>
#include <vector>

#include <header.hpp>

#include "graphics_shader.hpp"

SNOW_OWL_NAMESPACE(gx)

namespace pipeline {

  enum class raster_fill_mode {
    modeWireFrame,
    modeFill
  };

  enum class raster_cull_mode {
    modeNone,
    modeFront,
    modeBack
  };

  enum class blend_type {
    typeZero,
    typeOne,
    typeSourceColor,
    typeSourceColorInvert,
    typeSourceAlpha,
    typeSourceAlphaInvert,
    typeDestinationColor,
    typeDestinationColorInvert,
    typeDestinationAlpha,
    typeDestinationAlphaInvert,
    typeBlendFactor,
    typeBlendFactorInvert,
  };

  enum class blend_op {
    opAdd,
    opSubtract,
    opMin,
    opMax
  };

  enum class topology_type {
    typeTriangleList,
    typeTriangleFan,
    typeTriangleStrip
  };

  struct write_mask {

    operator uint16_t() const;

    bool operator&(const write_mask& rhs);

    uint16_t value;

    static write_mask r;
    static write_mask g;
    static write_mask b;
    static write_mask a;
  };

  struct shader_stage {

    operator uint16_t() const;

    uint16_t value;

    static shader_stage vertex;
    static shader_stage fragment;

  };

  struct raster {

    raster_fill_mode fill_mode;
    raster_cull_mode cull_mode;

    bool render_counter_clockwise;
  };

  struct blend {

    bool          enabled;

    blend_type    source_blend;
    blend_type    destination_blend;

    blend_op      blend_operation;

    blend_type    source_alpha_blend;
    blend_type    destination_alpha_blend;

    blend_op      blend_alpha_operation;

    write_mask    write_mask;
  };

  struct depth {
    bool enabled;

    int format;
  };

  struct stencil {
    bool enabled;
  };

  struct sample {
    unsigned int count;
    unsigned int quality;
  };

  struct render_output {
    int             format;
    pipeline::blend blend;
  };

  struct render_input_item {
    int             format;
    int             type;
    int             location;
    int             scope;
  };

  struct render_input {
    std::vector<render_input_item> resource_binding;
  };
}

struct graphics_render_pipeline {

  std::array<graphics_shader, 2>          shader_stages;

  pipeline::raster                        raster;
  pipeline::depth                         depth;
  pipeline::stencil                       stencil;
  pipeline::sample                        sample;
  pipeline::topology_type                 topology_type;

  bool                                    independent_target_blend;

  std::array<pipeline::render_input, 2>   render_inputs;
  std::array<pipeline::render_output, 8>  render_outputs;
};

SNOW_OWL_NAMESPACE_END
