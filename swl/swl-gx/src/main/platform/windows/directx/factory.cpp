#include "directx/factory.h"

SNOW_OWL_NAMESPACE(gx)

typedef factory < dx::context, 0 > fx;

fx::factory             (dx::context&& instance)
  : instance(std::move(instance)) {}

fx::Queue
  fx::queue             () { return Queue(instance); }

fx::SwapChain
  fx::swap_chain        (Queue &queue, ui::window &window) {
  return SwapChain(instance, queue, window);
}

fx::RenderBlock
  fx::render_block      (Queue &queue, RenderPipeline *pipeline) {
  return RenderBlock(queue, pipeline);
}

fx::RenderPass
  fx::render_pass       (RenderBlock &block, std::array < pipeline::pass_output, NRS > const &pass_context) {
  return RenderPass(block, pass_context);
}

fx::RenderPipeline
  fx::render_pipeline   () {
  return RenderPipeline(instance);
}

fx::BufferAllocator
  fx::buffer_allocator  (size_t initial_size) {
  return BufferAllocator(instance, initial_size);
}

SNOW_OWL_NAMESPACE_END
