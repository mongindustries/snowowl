//
// Created by Michael Ong on 25/4/20.
//
#include "metal/mtl_queue.h"

SNOW_OWL_NAMESPACE(gx::mtl)

mtl_queue::mtl_queue(const cx::exp::ptr_ref<mtl_context> &context): graphics_queue(context.cast<graphics_context>()),
	handle(cx::core::make_handle()), fence(0), buffer([context->queue commandBuffer]), event([context->device newEvent]) {
	event.label = [@"gx::mtl_queue " stringByAppendingString:@(handle).stringValue];
}

void mtl_queue::begin   () {
	[buffer encodeWaitForEvent:event value:fence];
}

void mtl_queue::submit  () {

	const uint64_t  fence_value  = fence;
									fence       += 1;

	[buffer commit];
	[buffer encodeSignalEvent:event value:fence_value];
}

SNOW_OWL_NAMESPACE_END
