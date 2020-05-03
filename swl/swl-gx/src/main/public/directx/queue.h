#pragma once

#include <atomic>

#include <header.hpp>

#include "context.h"
#include "queue.hpp"

SNOW_OWL_NAMESPACE(gx::dx)

struct queue final : gx::queue {

  explicit queue  (dx::context& context);


  void  begin     (std::vector<cx::exp::ptr_ref<gx::queue>> const& dependencies) override;

  void  submit    (std::vector<cx::exp::ptr_ref<gx::render_block>> const& commands) override;


  std::atomic<uint64_t>                   fence_frame;

  HANDLE                                  wait;


  winrt::com_ptr<ID3D12CommandQueue>      command_queue;

  winrt::com_ptr<ID3D12CommandAllocator>  command_allocator;

  winrt::com_ptr<ID3D12Fence>             fence;
};

SNOW_OWL_NAMESPACE_END
