#pragma once

#include <atomic>

#include <header.hpp>

#include "context.h"
#include "graphics_queue.hpp"

SNOW_OWL_NAMESPACE(gx::dx)

struct queue final : graphics_queue {

  queue   (const cx::exp::ptr_ref<dx::context>& context);

  ~queue  () override;


  cx::exp::ptr<graphics_render_block>
        create_render_block (const cx::exp::ptr_ref<graphics_render_pipeline>& pipeline) override;


  void  begin               (const std::vector<cx::exp::ptr_ref<graphics_queue>>& dependencies) override;

  void  submit              (const std::vector<cx::exp::ptr_ref<graphics_render_block>>& commands) override;


  std::atomic<uint64_t>              fence_frame;

  HANDLE                             wait;


  winrt::com_ptr<ID3D12CommandQueue>      command_queue;

  winrt::com_ptr<ID3D12CommandAllocator>  command_allocator;

  winrt::com_ptr<ID3D12Fence>             fence;
};

SNOW_OWL_NAMESPACE_END
