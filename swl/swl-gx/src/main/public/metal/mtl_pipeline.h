//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#import <Metal/Metal.h>

#import <header.hpp>
#import <ownership_exp.hpp>

#import "graphics_pipeline.hpp"

#import "mtl_context.h"
#import "mtl_queue.h"

SNOW_OWL_NAMESPACE(gx::mtl)

struct mtl_pipeline: graphics_pipeline {

	mtl_pipeline(
		const cx::exp::ptr_ref<mtl_context>&    context,
		const cx::exp::ptr_ref<mtl_queue>&      queue,
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
