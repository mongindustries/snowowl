#pragma once

#include <header.hpp>

#include "directx/context.h"
#include "directx/pipeline.h"

#include "render_block.hpp"
#include "render_pass.hpp"

SNOW_OWL_NAMESPACE(gx::dx)

struct queue;

struct render_block final : gx::render_block {

  render_block        (dx::queue& queue, dx::render_pipeline& pipeline);


  void  reset         (gx::render_pipeline& pipeline) override;

  void  close         () override;


  winrt::com_ptr<ID3D12CommandAllocator>      allocator;

  winrt::com_ptr<ID3D12GraphicsCommandList4>  command_list;
};

struct render_pass final : gx::render_pass {

  render_pass   (dx::render_block& block, std::vector<gx::render_pass_context> const& context);

  ~render_pass  ();


  winrt::com_ptr<ID3D12GraphicsCommandList4>  command_list;

  std::vector<D3D12_RESOURCE_BARRIER>         to_barriers;
};

SNOW_OWL_NAMESPACE_END
