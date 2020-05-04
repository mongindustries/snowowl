#import <QuartzCore/QuartzCore.h>

#import <window_surface.hpp>
#import <Metal/Metal.h>

#import "metal/context.h"
#import "metal/queue.h"
#import "metal/swap_chain.h"

SNOW_OWL_NAMESPACE(gx::mtl)

swap_chain::swap_chain(gx::context &context, gx::queue &present_queue, ui::window &window):
  gx::swap_chain(context, present_queue, window),
  present_queue (&static_cast<mtl::queue&>(present_queue)) {

  ui::window_surface surface{ cx::exp::ptr_ref{ &window } };

  CAMetalLayer* layer_ref = (__bridge CAMetalLayer*) surface.cast<void>().pointer();

  layer_ref.device        = static_cast<mtl::context&>(context).device;
  layer_ref.drawableSize  = CGSizeMake(window.get_size().x(), window.get_size().y());
  layer_ref.pixelFormat   = MTLPixelFormatBGRA8Unorm;

  frames.reserve(layer_ref.maximumDrawableCount);

  for (uint16_t index = 0u; index < layer_ref.maximumDrawableCount; index += 1) {

    mtl_frame* frame_item = new mtl_frame();

    frame_item->index    = index;
    frame_item->drawable = nil;

    frames.emplace_back(frame_item);
  }
}

cx::exp::ptr_ref<gx::swap_chain::frame>
  swap_chain::next_frame  () {

  mtl_frame&  frame           = static_cast<mtl_frame&>(frames[current_frame % frames.size()].get());
              frame.drawable  = [layer nextDrawable];

  return cx::exp::ptr_ref<gx::swap_chain::frame>{ nullptr };
}

void
  swap_chain::present     (std::vector<cx::exp::ptr_ref<gx::queue>> const& dependencies) {

  mtl_frame&      frame     = static_cast<mtl_frame&>(frames[current_frame % frames.size()].get());
  id<MTLDrawable> drawable  = frame.drawable;

  frame.drawable = nil;

  [present_queue->current_buffer presentDrawable:drawable];
  current_frame += 1;
}

void
  swap_chain::resize      (cx::size_2d const& new_size) {
  layer.drawableSize = CGSizeMake(new_size.x(), new_size.y());
}

SNOW_OWL_NAMESPACE_END
