//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#include <header.hpp>
#include <graphics_pass.hpp>

#include "mtl_context.h"
#include "mtl_queue.h"
#include "mtl_pipeline.h"

SNOW_OWL_NAMESPACE(gx::mtl)

struct mtl_pass: graphics_pass {

	mtl_pass(
		const cx::exp::ptr_ref<mtl_context>&   context ,
		const cx::exp::ptr_ref<mtl_queue>&     queue   ,
		const cx::exp::ptr_ref<mtl_pipeline>&  pipeline);
};

SNOW_OWL_NAMESPACE_END
