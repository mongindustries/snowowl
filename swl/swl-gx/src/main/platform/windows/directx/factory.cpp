#include "directx/factory.h"

SNOW_OWL_NAMESPACE(gx)

typedef factory < dx::context, 0 > fx;

fx::t_queue
  fx::queue() { return fx::t_queue(instance); }

fx::t_swap_chain
  fx::swap_chain(fx::t_queue &queue, ui::window &window) { return fx::t_swap_chain(instance, queue, window); }

fx::t_render_block
  fx::render_block(t_queue &queue, t_render_pipeline *pipeline) { return fx::t_render_block(queue, pipeline); }

fx::t_render_pass
  fx::render_pass(t_render_block &block, std::vector < gx::render_pass_context > const &pass_context) { return fx::t_render_pass(block, pass_context); }

fx::t_render_pipeline
  fx::render_pipeline() { return t_render_pipeline(instance); }

cx::exp::ptr < buffer_allocator >
  fx::buffer_allocator(size_t initial_size) { return cx::exp::ptr < gx::buffer_allocator >{new dx::buffer_allocator(instance, initial_size)}; }

SNOW_OWL_NAMESPACE_END
