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

enum class blend_type       {
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

enum class topology_type    {
	typeTriangleList,
	typeTriangleFan,
	typeTriangleStrip
};

struct write_mask {

	operator uint16_t() const;

	bool operator&(const write_mask& rhs);

	uint16_t value;
};

write_mask write_mask_r = { 0b0001 };
write_mask write_mask_g = { 0b0010 };
write_mask write_mask_b = { 0b0100 };
write_mask write_mask_a = { 0b1000 };

struct shader_stage {

	operator uint16_t() const;

	uint16_t value;
};

shader_stage vertex_stage   = { 0b0001 };
shader_stage fragment_stage = { 0b0010 };

struct raster           {

	raster_fill_mode fill_mode;
	raster_cull_mode cull_mode;

	bool render_counter_clockwise;
};

struct blend            {

	bool          enabled;

	blend_type    source_blend;
	blend_type    destination_blend;

	blend_op      blend_operation;

	blend_type    source_alpha_blend;
	blend_type    destination_alpha_blend;

	blend_op      blend_alpha_operation;

	write_mask    write_mask;
};

struct depth            {
	bool enabled;

	int format;
};

struct stencil          {
	bool enabled;
};

struct sample           {
	unsigned int count;
	unsigned int quality;
};

struct render_output    {
	int             format;
	pipeline::blend blend;
};
}

struct graphics_render_pipeline {

	graphics_render_pipeline(
			std::array<graphics_shader, 2>          shader_stages,
			pipeline::raster                        raster,
			pipeline::depth                         depth,
			pipeline::stencil                       stencil,
			pipeline::sample                        sample,
			bool                                    independent_blend,
			pipeline::topology_type                 topology_type,
			std::array<pipeline::render_output, 8>  render_outputs);
};

SNOW_OWL_NAMESPACE_END
