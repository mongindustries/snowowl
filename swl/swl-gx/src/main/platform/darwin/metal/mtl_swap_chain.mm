//
// Created by Michael Ong on 25/4/20.
//
#import "metal/mtl_swap_chain.h"
#import "metal/mtl_queue.h"

#import <window_surface.hpp>

SNOW_OWL_NAMESPACE(gx::mtl)

mtl_swap_chain::mtl_swap_chain
	(const cx::exp::ptr_ref<mtl_context> &ctx, const cx::exp::ptr_ref<mtl_queue>& queue, const cx::exp::ptr_ref<ui::window> &window):
	graphics_swap_chain(ctx.cast<graphics_context>(), queue.cast<graphics_queue>(), window), layer(nil), present_queue(queue) {

	layer             = (__bridge CAMetalLayer*) ui::window_surface(window).cast<void>().pointer();

	layer.device                = ctx->device;
	layer.pixelFormat           = MTLPixelFormatBGRA8Unorm;
	layer.maximumDrawableCount  = 3;

	frames.reserve(3);

	for(auto index = 0u; index < layer.maximumDrawableCount; index += 1) {

		mtl_frame frame;

		frame.id          = 0;
		frame.index       = index;
		frame.texture     = nil;
		frame.swap_chain  = cx::exp::ptr_ref<graphics_swap_chain>{ this };

		auto obj = cx::exp::ptr<gx::graphics_swap_chain::frame, mtl_frame>(std::move(frame));

		frames.emplace_back(obj.abstract_and_release());
	}

	std::function<void(const ui::window&, const cx::rect&)> func = [&](const ui::window&, const cx::rect& size) {
		this->layer.drawableSize = CGSizeMake(size.size.x(), size.size.y());
	};

	window->event_on_resize.emplace_back(func);
}

cx::exp::ptr_ref<graphics_swap_chain::frame>
			mtl_swap_chain::next_frame  () {

	if (!layer) {
		return cx::exp::ptr_ref<graphics_swap_chain::frame>{ nullptr };
	}

	auto drawable = [layer nextDrawable];
	auto frame    = cx::exp::ptr_ref<graphics_swap_chain::frame>{ frames[cur_frame] }.cast<mtl_frame>();

	if (@available(macOS 10.15.4, *)) {
	    frame->id = static_cast<unsigned int>(drawable.drawableID);
	}

	frame->texture  = drawable.texture;
	frame->drawable = drawable;

	cur_frame += 1;
	cur_frame  = cur_frame % frames.size();

	return cx::exp::ptr_ref<mtl_frame>{ frame }.cast<graphics_swap_chain::frame>();
}

void  mtl_swap_chain::present     () {

	auto frame  = cx::exp::ptr_ref<graphics_swap_chain::frame> { frames[cur_frame] }.cast<mtl_frame>();
	[present_queue->buffer presentDrawable:frame->drawable];

	frame->drawable = nil;
	frame->texture  = nil;
}

SNOW_OWL_NAMESPACE_END
