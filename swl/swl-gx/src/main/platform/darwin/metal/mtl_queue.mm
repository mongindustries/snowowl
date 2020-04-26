//
// Created by Michael Ong on 25/4/20.
//
#include "metal/mtl_queue.h"

SNOW_OWL_NAMESPACE(gx::mtl)

mtl_queue::mtl_queue    (const cx::exp::ptr_ref<mtl_context> &context): graphics_queue(context.cast<graphics_context>()),
	handle(cx::core::make_handle()), fence(0), buffer([context->queue commandBuffer]), event([context->device newEvent]) {
	event.label = [@"gx::mtl_queue " stringByAppendingString:@(handle).stringValue];
}

void mtl_queue::begin   (const std::vector<cx::exp::ptr_ref<graphics_queue>>& dependencies) {
	[buffer encodeWaitForEvent:event value:fence]; // wait for previous buffer execution

	for (auto& queue : dependencies) {
		const auto casted = queue.cast<mtl_queue>();

		// wait for the dependency's execution to be finished. if their submit method was called
		// the fence value for this queue should be value + 1 on the time of submission.
		// if the buffer finished before this buffer can begin its execution, this will do nothing.
		[casted->buffer encodeWaitForEvent:casted->event value:casted->fence];
	}
}

void mtl_queue::submit  (const std::vector<cx::exp::ptr_ref<graphics_render_pass>>& passes) {

	fence += 1;

	(void) passes; // unused

	[buffer encodeSignalEvent:event value:fence];
	[buffer commit];
}

SNOW_OWL_NAMESPACE_END
