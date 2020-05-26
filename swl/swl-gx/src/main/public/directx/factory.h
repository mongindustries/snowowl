#pragma once

#include <header.hpp>

#include "factory.hpp"

#include "directx/context.h"
#include "directx/queue.h"
#include "directx/swap_chain.h"
#include "directx/render_block.h"
#include "directx/render_pass.h"
#include "directx/render_pipeline.h"
#include "directx/buffer_allocator.h"

SNOW_OWL_NAMESPACE(gx)

template <>
struct factory < dx::context > {

  typedef dx::queue             Queue;
  typedef dx::swap_chain        SwapChain;
  
  typedef dx::render_block      RenderBlock;
  typedef dx::render_pass       RenderPass;
  typedef dx::render_pipeline   RenderPipeline;

  typedef dx::buffer_allocator  BufferAllocator;

  explicit
    factory           (dx::context &&instance);

  [[nodiscard]] SwapChain
    swap_chain        (Queue &queue, ui::window &window);

  [[nodiscard]] Queue
    queue             ();

  [[nodiscard]] RenderBlock
    render_block      (Queue &queue, RenderPipeline *pipeline);

  [[nodiscard]] RenderPass
    render_pass       (RenderBlock &block, std::array< pipeline::pass_output, NRS > const &pass_context);

  [[nodiscard]] RenderPipeline
    render_pipeline   ();
  [[nodiscard]] BufferAllocator
    buffer_allocator  (size_t initial_size);

private:

  dx::context instance;
};

SNOW_OWL_NAMESPACE_END
