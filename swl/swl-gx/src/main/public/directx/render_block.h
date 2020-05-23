#pragma once

#include <header.hpp>

#include "directx/context.h"
#include "directx/pipeline.h"

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


  winrt::com_ptr < ID3D12CommandAllocator > allocator;

  winrt::com_ptr < ID3D12GraphicsCommandList4 > command_list;

  cx::exp::ptr_ref < dx::render_pipeline > current_pipeline;
};

struct render_pass final : gx::render_pass {

  render_pass(dx::render_block &block, std::vector < gx::render_pass_context > const &context);

  ~render_pass();


  void
    set_viewport(const cx::size_2d &value) override;

  void
    set_scissor(const cx::rect &value) override;

  void
    set_topology(topology_type type) override;

  cx::exp::ptr < gx::transition_handle >
    buffer_boundary(std::vector < std::pair < cx::exp::ptr_ref < gx::resource_reference >, buffer_transition > > const &transitions) override;

  void
    bind_buffer(render_pass_stage_binding binding, int slot, cx::exp::ptr_ref < gx::resource_reference > const &reference) override;

  void
    bind_buffers(render_pass_stage_binding binding, std::array < cx::exp::ptr_ref < gx::resource_reference >, 16 > const &references) override;

  void
    draw(const render_pass_draw_range &vertex_range) override;


  winrt::com_ptr < ID3D12GraphicsCommandList4 > command_list;

  std::vector < D3D12_RESOURCE_BARRIER > to_barriers;

  cx::exp::ptr_ref< dx::render_pipeline > pipeline;
};

SNOW_OWL_NAMESPACE_END
