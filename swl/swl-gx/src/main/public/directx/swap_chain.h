#pragma once

#include <header.hpp>

#include "graphics_swap_chain.hpp"

#include "directx/context.h"
#include "directx/queue.h"

SNOW_OWL_NAMESPACE(gx::dx)

struct render_block;

struct swap_chain : graphics_swap_chain {

  swap_chain    (const cx::exp::ptr_ref<dx::context>& context, const cx::exp::ptr_ref<dx::queue>& present_queue, const cx::exp::ptr_ref<ui::window>& window);

  ~swap_chain   () override;

  cx::exp::ptr_ref<frame>
        next_frame  () override;

  void  present     (std::vector<cx::exp::ptr_ref<graphics_queue>> const& dependencies) override;


  void  resize      (const cx::size_2d& new_size) override;


  HANDLE                                    event_wait;


  uint64_t                                  current_frame;

  winrt::com_ptr<ID3D12Fence>               frame_fence;

  bool                                      needs_resize{ false };


  cx::exp::ptr_ref<dx::queue>               present_queue;


  winrt::com_ptr<IDXGISwapChain3>           instance;

  winrt::com_ptr<ID3D12DescriptorHeap>      frame_heap;
};

SNOW_OWL_NAMESPACE_END
