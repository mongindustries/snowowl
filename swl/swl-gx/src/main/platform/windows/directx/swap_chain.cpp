#include "directx/swap_chain.h"
#include "directx/resource_reference.h"
#include "directx/render_block.h"

#include <sstream>

#include <window_surface.hpp>

#include <swl_window_backend.hpp>
#include <../platform/windows/swl_win32_window.hpp>

#ifdef SWL_UWP

#include <Unknwn.h>
#include <winrt/Windows.UI.Core.h>

using namespace winrt;

#endif

SNOW_OWL_NAMESPACE(gx::dx)

template<typename C>
using ptr_ref = cx::exp::ptr_ref<C>;

swap_chain::swap_chain(context &context, queue &present_queue, ui::window &window): gx::swap_chain(context, present_queue, window),
  event_wait          (CreateEvent(nullptr, false, false, L"Swap chain resize wait event")),
  current_frame       (0),
  present_queue       (&present_queue) {

  window.swap_chain = ptr_ref<gx::swap_chain>{this };

  DXGI_SWAP_CHAIN_DESC1 swap_chain_desc{};

  swap_chain_desc.Width       = window.get_size().x();
  swap_chain_desc.Height      = window.get_size().y();

  swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

  swap_chain_desc.Format      = DXGI_FORMAT_B8G8R8A8_UNORM;
  swap_chain_desc.BufferCount = 3;

  swap_chain_desc.Scaling     = DXGI_SCALING_STRETCH;
  swap_chain_desc.SwapEffect  = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

  swap_chain_desc.AlphaMode   = DXGI_ALPHA_MODE_IGNORE;

  swap_chain_desc.SampleDesc.Count    = 1;
  swap_chain_desc.SampleDesc.Quality  = 0;

  const ui::window_surface surface{ cx::exp::ptr_ref<ui::window>{ &window } };
  const void* window_handle = surface.cast<void>().pointer();

  winrt::com_ptr<IDXGISwapChain1> _pre_instance;

#ifdef SWL_UWP
  context.dxgi_factory->CreateSwapChainForCoreWindow(present_queue.command_queue.get(), (::IUnknown*) window_handle, &swap_chain_desc, nullptr, _pre_instance.put());
#else
  context.dxgi_factory->CreateSwapChainForHwnd(present_queue.command_queue.get(), HWND(window_handle), &swap_chain_desc, nullptr, nullptr, _pre_instance.put());
#endif

  _pre_instance->QueryInterface(__uuidof(IDXGISwapChain3), instance.put_void());

  frames.reserve(swap_chain_desc.BufferCount);

  D3D12_DESCRIPTOR_HEAP_DESC frameHeapDesc{};

  frameHeapDesc.NumDescriptors  = swap_chain_desc.BufferCount;
  frameHeapDesc.Type            = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

  auto& device = context.device;

  device->CreateDescriptorHeap(&frameHeapDesc, __uuidof(ID3D12DescriptorHeap), frame_heap.put_void());

  const SIZE_T offset_size  = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
  auto heap_start   = frame_heap->GetCPUDescriptorHandleForHeapStart();

  for (auto i = 0U; i < swap_chain_desc.BufferCount; i += 1) {

    swap_chain::frame frame{};

    frame.index       = i;
    frame.swap_chain  = cx::exp::ptr_ref{ this }.cast<gx::swap_chain>();

    dx::resource_reference ref;

    ref.handle.cpu_handle = heap_start;
    ref.type              = typeCPU;
    ref.created_state     = D3D12_RESOURCE_STATE_PRESENT;
    ref.format            = swap_chain_desc.Format;

    _pre_instance->GetBuffer(i, __uuidof(ID3D12Resource), ref.resource.put_void());
    device->CreateRenderTargetView(ref.resource.get(), nullptr, heap_start);

    ref.resource->SetName((std::wstringstream() << L"Swap chain back buffer index " << i).str().c_str());

    frame.reference       = cx::exp::ptr<gx::resource_reference, dx::resource_reference>{std::move(ref)};

    frames.emplace_back(std::move(frame));

    heap_start = D3D12_CPU_DESCRIPTOR_HANDLE{ heap_start.ptr + offset_size };
  }
}


void
  swap_chain::resize      (cx::size_2d const& new_size) {

  DXGI_SWAP_CHAIN_DESC1 desc{};
  instance->GetDesc1(&desc);

  needs_resize = new_size.x() != desc.Width || new_size.y() != desc.Height;
}

cx::exp::ptr_ref<swap_chain::frame>
  swap_chain::next_frame  () {

  if (needs_resize) {
    needs_resize = false;

    for (auto& frame : frames) {

      auto ref = cx::exp::ptr_ref{ frame->reference }.cast<dx::resource_reference>();

      ref->handle.cpu_handle = {};

      if (winrt::com_ptr<struct ID3D12Resource>::type* resource = ref->resource.detach()) {
        resource->Release();
      }
    }

    const auto new_size = window->get_size();
    instance->ResizeBuffers(0, new_size.x(), new_size.y(), DXGI_FORMAT_UNKNOWN, 0);

    winrt::com_ptr<ID3D12Device> device;
    instance->GetDevice(__uuidof(ID3D12Device), device.put_void());

    const SIZE_T offset_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    auto   heap_start = frame_heap->GetCPUDescriptorHandleForHeapStart();

    for (auto i = 0U; i < frames.size(); i += 1) {

      auto reference = cx::exp::ptr_ref{ frames[i]->reference }.cast<dx::resource_reference>();
      instance->GetBuffer(i, __uuidof(ID3D12Resource), reference->resource.put_void());

      reference->resource->SetName((std::wstringstream() << L"Swap chain back buffer index " << i).str().c_str());

      reference->handle.cpu_handle = heap_start;
      device->CreateRenderTargetView(reference->resource.get(), nullptr, heap_start);

      heap_start = D3D12_CPU_DESCRIPTOR_HANDLE{ heap_start.ptr + offset_size };
    }
  }

  return cx::exp::ptr_ref<swap_chain::frame>{frames[instance->GetCurrentBackBufferIndex()] };
}

void
  swap_chain::present     (std::vector<ptr_ref<gx::queue>> const& dependencies) {

  assert(instance->Present(swaps_immediately ? 0 : 1, 0) == S_OK);
}

SNOW_OWL_NAMESPACE_END
