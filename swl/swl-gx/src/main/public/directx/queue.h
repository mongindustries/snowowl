#pragma once

#include <atomic>

#include <header.hpp>

#include "context.h"
#include "render_block.h"

#include "queue.hpp"

SNOW_OWL_NAMESPACE(gx::dx)

struct queue final : gx::queue {

  explicit
    queue   (dx::context &context);

  ~queue    () override;


  void
    begin     (std::vector < std::reference_wrapper < gx::queue > > const &dependencies) override;


  void
    submit    (std::vector < std::reference_wrapper < gx::render_block > > const &commands) override;

  void
    transfer  (std::vector < cx::exp::ptr_ref < transfer_block > > const &buffers) override;


  std::atomic < uint64_t >  fence_frame;

  HANDLE                    wait;


  winrt::com_ptr < ID3D12CommandQueue >         command_queue;

  winrt::com_ptr < ID3D12CommandAllocator >     command_allocator;
  winrt::com_ptr < ID3D12CommandAllocator >     transfer_allocator;

  winrt::com_ptr < ID3D12GraphicsCommandList >  command_list_transfer;

  winrt::com_ptr < ID3D12Fence >                fence;
};

SNOW_OWL_NAMESPACE_END
