//
// Created by Michael Ong on 27/4/20.
//

#define FROM_BLOCK

#import <Metal/Metal.h>

#import "metal/mtl_render_block.h"
#import "metal/mtl_render_pass.h"

SNOW_OWL_NAMESPACE(gx::mtl)

template<typename c>
using ptr_ref = cx::exp::ptr_ref<c>;

mtl_render_block::mtl_render_block  (const ptr_ref<mtl_queue> &queue):
	graphics_render_block(), queue(queue) {

}

void  mtl_render_block::render_pass (const std::function<void(graphics_render_pass &)> &configure) {

	auto descriptor = [MTLRenderPassDescriptor new];

	auto encoder = [queue->buffer renderCommandEncoderWithDescriptor:descriptor];

	mtl_render_pass render_pass{ encoder };
	configure(render_pass);

	[encoder endEncoding];
}

void  mtl_render_block::begin       () {
	// do nothing
}

void  mtl_render_block::end         () {
	[queue->buffer enqueue];
}


SNOW_OWL_NAMESPACE_END
