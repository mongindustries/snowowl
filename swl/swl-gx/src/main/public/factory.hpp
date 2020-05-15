//
// Created by micha on 5/3/2020.
//
#pragma once

#include <type_traits>

#include <header.hpp>
#include <ownership.hpp>

#include "context.hpp"
#include "queue.hpp"
#include "swap_chain.hpp"
#include "render_block.hpp"
#include "render_pass.hpp"
#include "render_pipeline.hpp"

#include "buffer.hpp"
#include "buffer_allocator.hpp"

SNOW_OWL_NAMESPACE(gx)

template < typename context, std::enable_if_t < std::is_base_of_v < gx::context, context >, int > = 0 >
struct factory {

  typedef gx::queue                t_queue;
  typedef cx::exp::ptr < t_queue > p_queue;

  typedef gx::swap_chain                t_swap_chain;
  typedef cx::exp::ptr < t_swap_chain > p_swap_chain;

  typedef gx::render_block                t_render_block;
  typedef cx::exp::ptr < t_render_block > p_render_block;

  typedef gx::render_pass                t_render_pass;
  typedef cx::exp::ptr < t_render_pass > p_render_pass;

  typedef gx::render_pipeline                t_render_pipeline;
  typedef cx::exp::ptr < t_render_pipeline > p_render_pipeline;

  explicit
    factory(context &&instance)
    : instance(instance) {}

  [[nodiscard]] t_swap_chain
    swap_chain(t_queue &queue, ui::window &window) { return t_swap_chain(instance, queue, window); }

  [[nodiscard]] t_queue
    queue() { return t_queue(instance); }

  [[nodiscard]] t_render_block
    render_block(t_queue &queue, t_render_pipeline *pipeline) { return t_render_block(queue, pipeline); }

  [[nodiscard]] t_render_pass
    render_pass(t_render_block &block, std::vector < render_pass_context > const &pass_context) { return t_render_pass(isntance, block); }

  [[nodiscard]] t_render_pipeline
    render_pipeline() { return t_render_pipeline(instance); }

  [[nodiscard]] cx::exp::ptr < buffer_allocator >
    buffer_allocator(size_t initial_size) { return cx::exp::ptr{nullptr}; }

private:

  context instance;
};

SNOW_OWL_NAMESPACE_END
