//
// Created by Michael Ong on 25/4/20.
//
#import <Metal/Metal.h>

#import <chrono>
#import <iostream>
#import <cassert>

#import "metal/mtl_cpu_fence.h"
#import "metal/mtl_queue.h"

#import "graphics_render_block.hpp"

using namespace std::chrono_literals;

SNOW_OWL_NAMESPACE(gx::mtl)

template<typename c>
using ptr_ref = cx::exp::ptr_ref<c>;

mtl_queue::mtl_queue    (const ptr_ref<mtl_context> &context):
    graphics_queue(context.cast<graphics_context>()),
    context (context),
    handle  (cx::core::make_handle()),
    fence   (0),
    event   ([MTLCpuFence new]),
    buffer  (nil) { }

void mtl_queue::begin   (const std::vector<ptr_ref<graphics_queue>>& dependencies) {

    @autoreleasepool {

        for (auto& queue : dependencies) {
            const auto casted = queue.cast<mtl_queue>();

            // wait for the dependency's execution to be finished. if their submit method was called
            // the fence value for this queue should be value + 1 on the time of submission.
            // if the buffer finished before this buffer can begin its execution, this will do nothing.
            [casted->event cpuWait:casted->fence];
        }

        [event cpuWait:fence];
        buffer = [context->queue commandBuffer];
    }
}

void mtl_queue::submit  (const std::vector<ptr_ref<graphics_render_block>>& passes) {

    (void) passes; // unused

    @autoreleasepool {

        fence += 1;
        [buffer addCompletedHandler:^(id<MTLCommandBuffer> _Nonnull) {
            [event gpuSignal:fence];
        }];
        [buffer commit];
    }
}

SNOW_OWL_NAMESPACE_END
