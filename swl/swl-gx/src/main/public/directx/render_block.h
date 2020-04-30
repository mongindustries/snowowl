#pragma once

#include <header.hpp>

#include "directx/context.h"
#include "graphics_render_block.hpp"

SNOW_OWL_NAMESPACE(gx::dx)

struct render_block final : graphics_render_block {

  render_block        (const cx::exp::ptr_ref<context>& context, const cx::exp::ptr_ref<graphics_render_pipeline>& pipeline);


  void  reset         (const cx::exp::ptr_ref<graphics_render_pipeline>& pipeline) override;

  void  close         () override;


  void  render_pass   (const std::function<void(graphics_render_pass&)>& configure) override;


  winrt::com_ptr<ID3D12CommandAllocator>      allocator;

  winrt::com_ptr<ID3D12GraphicsCommandList4>  command_list;
};

SNOW_OWL_NAMESPACE_END
