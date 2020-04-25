//
// Created by Michael Ong on 25/4/20.
//
#import "metal/mtl_context.h"

#import "metal/mtl_swap_chain.h"
#import "metal/mtl_queue.h"

SNOW_OWL_NAMESPACE(gx::mtl)

mtl_context::mtl_context():
			device(MTLCreateSystemDefaultDevice()), queue([device newCommandQueue]) { }

cx::exp::ptr<graphics_swap_chain>
			mtl_context::create_swap_chain  (const cx::exp::ptr_ref<ui::window> &window) {
	return cx::exp::ptr<graphics_swap_chain, mtl_swap_chain>(new mtl_swap_chain(cx::exp::ptr_ref<mtl_context>{ this }, window)).abstract();
}

cx::exp::ptr<graphics_queue>
			mtl_context::create_queue       () {
	return cx::exp::ptr<graphics_queue, mtl_queue>(new mtl_queue(cx::exp::ptr_ref<mtl_context>{ this })).abstract();
}

SNOW_OWL_NAMESPACE_END
