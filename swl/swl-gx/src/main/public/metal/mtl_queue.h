//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#import <atomic>
#import <core.hpp>

#import "graphics_queue.hpp"
#import "mtl_context.h"

#ifdef __OBJC__
#import "mtl_cpu_fence.h"
#endif

SNOW_OWL_NAMESPACE(gx::mtl)

struct mtl_queue final: graphics_queue {

	mtl_queue     (const cx::exp::ptr_ref<mtl_context>& context);


	void  begin   (const std::vector<cx::exp::ptr_ref<graphics_queue>>& dependencies) override;

	void  submit  (const std::vector<cx::exp::ptr_ref<graphics_render_block>>& commands) override;

	cx::exp::ptr_ref<mtl_context>   context;

	cx::driver_handle               handle;

	std::atomic<uint64_t>           fence;

#if __OBJC__
	MTLCpuFence*                    event;
	id<MTLCommandBuffer>            buffer;
#else
	void*                           buffer;
	void*                           lock;
#endif
};

SNOW_OWL_NAMESPACE_END
