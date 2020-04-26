#pragma once

#include <header.hpp>

#include "graphics_render_pipeline.hpp"

#include "directx/context.h"

SNOW_OWL_NAMESPACE(gx::dx)

struct pipeline: graphics_pipeline {

	pipeline(
		const cx::exp::ptr_ref<dx::context>&        context,
		std::array<graphics_shader, 2>              shader_stages,
		gx::pipeline::raster                        raster,
		gx::pipeline::depth                         depth,
		gx::pipeline::stencil                       stencil,
		gx::pipeline::sample                        sample,
		bool                                        independent_blend,
		gx::pipeline::topology_type                 topology_type,
		std::array<gx::pipeline::render_output, 8>  render_outputs);


	winrt::com_ptr<ID3D12PipelineState> pipeline_state;
};

SNOW_OWL_NAMESPACE_END
