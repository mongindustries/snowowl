//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#ifdef FROM_BLOCK

#import <Metal/Metal.h>

#include <header.hpp>
#include <graphics_render_pass.hpp>

#include "mtl_context.h"
#include "mtl_queue.h"
#include "mtl_render_pipeline.h"

SNOW_OWL_NAMESPACE(gx::mtl)

struct mtl_render_pass: graphics_render_pass {

	mtl_render_pass(id<MTLRenderCommandEncoder> encoder);


	id<MTLRenderCommandEncoder> __weak  encoder;
};

SNOW_OWL_NAMESPACE_END

#endif
