#pragma once

#include <header.hpp>

#include "factory.hpp"

#include "directx/context.h"

#include "directx/swap_chain.h"
#include "directx/queue.h"
#include "directx/render_block.h"
#include "directx/buffer_allocator.h"

SNOW_OWL_NAMESPACE(gx)

template <>
struct factory < dx::context > {

  typedef dx::queue                t_queue;
  typedef cx::exp::ptr < t_queue > p_queue;

  typedef dx::swap_chain                t_swap_chain;
  typedef cx::exp::ptr < t_swap_chain > p_swap_chain;

  typedef dx::render_block                t_render_block;
  typedef cx::exp::ptr < t_render_block > p_render_block;

  typedef dx::render_pass                t_render_pass;
  typedef cx::exp::ptr < t_render_pass > p_render_pass;

  typedef dx::render_pipeline                t_render_pipeline;
  typedef cx::exp::ptr < t_render_pipeline > p_render_pipeline;

  explicit
    factory(dx::context &&instance)
    : instance(std::move(instance)) {}

  [[nodiscard]] t_swap_chain
    swap_chain(t_queue &queue, ui::window &window);

  [[nodiscard]] t_queue
    queue();

  [[nodiscard]] t_render_block
    render_block(t_queue &queue, t_render_pipeline *pipeline);

  [[nodiscard]] t_render_pass
    render_pass(t_render_block &block, std::vector < gx::render_pass_context > const &pass_context);

  [[nodiscard]] t_render_pipeline
    render_pipeline();

  [[nodiscard]] cx::exp::ptr < buffer_allocator >
    buffer_allocator(size_t initial_size);

private:

  dx::context instance;
};

SNOW_OWL_NAMESPACE_END
