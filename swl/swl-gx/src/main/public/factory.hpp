//
// Created by micha on 5/3/2020.
//
#pragma once

#include <filesystem>
#include <type_traits>

#include <header.hpp>

#include "context.hpp"
#include "queue.hpp"
#include "swap_chain.hpp"
#include "render_block.hpp"
#include "render_pass.hpp"
#include "render_pipeline.hpp"

#include "buffer_allocator.hpp"

SNOW_OWL_NAMESPACE(gx)

template < typename context, std::enable_if_t < std::is_base_of_v < gx::context, context >, int > = 0 >
struct factory {

  typedef gx::queue             Queue;
  typedef gx::swap_chain        SwapChain;
  
  typedef gx::render_block      RenderBlock;
  typedef gx::render_pass       RenderPass;
  typedef gx::render_pipeline   RenderPipeline;

  typedef gx::buffer_allocator  BufferAllocator;

  explicit
    factory(context &&instance)
    : instance(instance) {}

  [[nodiscard]] SwapChain
    swap_chain(Queue &queue, ui::window &window) {
    return SwapChain(instance, queue, window);
  }

  [[nodiscard]] Queue
    queue() {
    return Queue(instance);
  }

  [[nodiscard]] RenderBlock
    render_block(Queue &queue, RenderPipeline *pipeline) {
    return RenderBlock(queue, pipeline);
  }

  [[nodiscard]] RenderPass
    render_pass(RenderBlock &block, std::array< pipeline::pass_output, NRS > const &pass_context) {
    return RenderPass(instance, block);
  }

  [[nodiscard]] RenderPipeline
    render_pipeline() {
    return RenderPipeline(instance);
  }

  [[nodiscard]] BufferAllocator
    buffer_allocator(size_t initial_size) {
    return BufferAllocator(instance, initial_size);
  }

private:

  context instance;
};

SNOW_OWL_NAMESPACE_END
