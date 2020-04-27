//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#include <header.hpp>

#include "graphics_context.hpp"

SNOW_OWL_NAMESPACE(gx::mtl)

struct mtl_context: graphics_context {

	mtl_context();


	cx::exp::ptr<graphics_swap_chain>
		create_swap_chain (const cx::exp::ptr_ref<ui::window>& window, const cx::exp::ptr_ref<gx::graphics_queue>& present_queue) override;

	cx::exp::ptr<graphics_queue>
		create_queue      () override;

#ifdef __OBJC__
	id<MTLDevice>         device;

	id<MTLCommandQueue>   queue;
#else
	void*                 device;

	void*                 queue;
#endif
};

SNOW_OWL_NAMESPACE_END
