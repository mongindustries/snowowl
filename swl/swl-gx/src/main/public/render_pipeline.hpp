//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#include <array>
#include <vector>

#include <header.hpp>

#include "shader.hpp"

SNOW_OWL_NAMESPACE(gx)

struct context;

namespace pipeline {

  enum format {

    // standard formats

    format_4_32_int         = 0x04'04'01,
    format_4_32_int_u       = 0x04'04'03,
    format_4_32_float       = 0x04'04'05,

    format_3_32_int         = 0x03'04'01,
    format_3_32_int_u       = 0x03'04'03,
    format_3_32_float       = 0x03'04'05,

    format_2_32_int         = 0x02'04'01,
    format_2_32_int_u       = 0x02'04'03,
    format_2_32_float       = 0x02'04'05,

    format_1_32_int         = 0x01'04'01,
    format_1_32_int_u       = 0x01'04'03,
    format_1_32_float       = 0x01'04'05,


    format_4_16_int         = 0x04'02'01,
    format_4_16_int_norm    = 0x04'02'02,
    format_4_16_int_u       = 0x04'02'03,
    format_4_16_int_u_norm  = 0x04'02'04,
    format_4_16_float       = 0x04'02'05,

    format_2_16_int         = 0x02'02'01,
    format_2_16_int_norm    = 0x02'02'02,
    format_2_16_int_u       = 0x02'02'03,
    format_2_16_int_u_norm  = 0x02'02'04,
    format_2_16_float       = 0x02'02'05,

    format_1_16_int         = 0x01'02'01,
    format_1_16_int_norm    = 0x01'02'02,
    format_1_16_int_u       = 0x01'02'03,
    format_1_16_int_u_norm  = 0x01'02'04,
    format_1_16_float       = 0x01'02'05,

    format_4_8_int          = 0x04'01'01,
    format_4_8_int_norm     = 0x04'01'02,
    format_4_8_int_u        = 0x04'01'03,
    format_4_8_int_u_norm   = 0x04'01'04,

    format_2_8_int          = 0x02'01'01,
    format_2_8_int_norm     = 0x02'01'02,
    format_2_8_int_u        = 0x02'01'03,
    format_2_8_int_u_norm   = 0x02'01'04,

    format_1_8_int          = 0x01'01'01,
    format_1_8_int_norm     = 0x01'01'02,
    format_1_8_int_u        = 0x01'01'03,
    format_1_8_int_u_norm   = 0x01'01'04,

    // specialized formats

    // depth and stencil

    format_1_32_float_depth       = 0x01'D4'00,
    format_2_32_float_8_uint      = 0x02'D4'00,
    format_3_24_int_u_8_uint      = 0x03'D4'00,
    format_1_16_int_u_depth       = 0x04'D4'00,

    // swapchain

    format_4_8_int_flipped        = 0x01'B1'00,
    format_4_8_int_u_norm_flipped = 0x02'B1'00
  };

  enum class raster_fill_mode {
    modeWireFrame,
    modeFill
  };

  enum class raster_cull_mode {
    modeNone,
    modeFront,
    modeBack
  };

  enum blend_type {
    typeZero                    = 0x0000'0000,
    typeOne                     = 0x0000'0001,
    typeSourceColor             = 0x0001'0100,
    typeSourceColorInvert       = 0x0001'0101,
    typeSourceAlpha             = 0x0001'0200,
    typeSourceAlphaInvert       = 0x0001'0201,
    typeDestinationColor        = 0x0002'0100,
    typeDestinationColorInvert  = 0x0002'0101,
    typeDestinationAlpha        = 0x0002'0200,
    typeDestinationAlphaInvert  = 0x0002'0201,
    typeBlendFactor             = 0x0003'0100,
    typeBlendFactorInvert       = 0x0003'0101
  };

  enum blend_op {
    opAdd,
    opSubtract,
    opMin,
    opMax
  };

  enum topology_type {
    topologyTypePoint,
    topologyTypeLine,
    topologyTypeTriangle
  };

  enum render_input_type {
    typeTexture,
    typeBuffer
  };

  struct write_mask {

    operator uint8_t() const;

    bool operator&(const write_mask& rhs) const;

    uint8_t value;

    static write_mask r;
    static write_mask g;
    static write_mask b;
    static write_mask a;
  };

  struct shader_stage {

    operator uint8_t() const;

    uint8_t value;

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
    blend_type    source_alpha_blend;

    blend_type    destination_blend;
    blend_type    destination_alpha_blend;

    bool          op_enabled;

    blend_op      blend_operation;
    blend_op      blend_alpha_operation;

    blend_op      logic_op;

    write_mask    write_mask;
  };

  struct depth {
    bool    enabled;
    format  format;
  };

  struct stencil {
    bool    enabled;
  };

  struct sample {
    unsigned int count;
    unsigned int quality;
  };

  struct render_output {
    format          format;
    pipeline::blend blend;
  };

  struct render_input_item {
    format            format;
    render_input_type type;
    int               location;
    int               region;
  };

  struct render_input {
    std::vector<render_input_item> resource_binding;
  };
}

struct render_pipeline { SWL_REFERENCEABLE(render_pipeline) SWL_POLYMORPHIC(render_pipeline)

  render_pipeline(context& context);

  std::array<shader, 2>                   shader_stages;

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
