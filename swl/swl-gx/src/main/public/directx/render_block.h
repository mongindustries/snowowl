#pragma once

#include <header.hpp>

#include "directx/context.h"
#include "directx/render_pipeline.h"

#include "render_block.hpp"
#include "render_pass.hpp"

SNOW_OWL_NAMESPACE(gx::dx)

struct queue;

struct render_block final : public gx::render_block {

  render_block(dx::queue &queue, dx::render_pipeline *pipeline);


  void
    reset(gx::render_pipeline &pipeline) override;

  void
    close() override;


  winrt::com_ptr < ID3D12CommandAllocator >     allocator;

  winrt::com_ptr < ID3D12GraphicsCommandList4 > command_list;

  cx::exp::ptr_ref < dx::render_pipeline >      current_pipeline;
};

SNOW_OWL_NAMESPACE_END
