#include "directx/swap_chain.h"
#include "directx/resource_reference.h"

#include <sstream>

#include <window.hpp>
#include "../../../../swl-ui/src/main/platform/windows/swl_internal_state.h"

using namespace winrt;

SNOW_OWL_NAMESPACE(gx::dx)

template < typename C >
using ptr_ref = cx::exp::ptr_ref < C >;

swap_chain::swap_chain(context &context, queue &present_queue, ui::window &window)
  : gx::swap_chain(context, present_queue, window)
  , event_wait(CreateEvent(nullptr, false, false, L"Swap chain resize wait event"))
  , current_frame(0)
  , present_queue(&present_queue) {

  assert(!window.state()->swap_chain);

  window.state()->swap_chain = cx::exp::ptr_ref<gx::swap_chain>{ this };

  DXGI_SWAP_CHAIN_DESC1 swap_chain_desc{};

  swap_chain_desc.Width  = window.size().x();
  swap_chain_desc.Height = window.size().y();

  swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

  swap_chain_desc.Format      = DXGI_FORMAT_B8G8R8A8_UNORM;
  swap_chain_desc.BufferCount = 3;

  swap_chain_desc.Scaling    = DXGI_SCALING_STRETCH;
  swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

  swap_chain_desc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

  swap_chain_desc.SampleDesc.Count   = 1;
  swap_chain_desc.SampleDesc.Quality = 0;

  com_ptr < IDXGISwapChain1 > pre_instance;

#ifdef SWL_UWP
  const auto window_handle = surface.cast < IUnknown >().pointer();
  context.dxgi_factory->CreateSwapChainForCoreWindow(present_queue.command_queue.get(), window_handle, &swap_chain_desc,
                                                     nullptr, pre_instance.put());
#else
  const auto window_handle = window.state()->handle;
  context.dxgi_factory->CreateSwapChainForHwnd(present_queue.command_queue.get(), window_handle, &swap_chain_desc, nullptr, nullptr, pre_instance.put());
#endif

  pre_instance->QueryInterface(__uuidof(IDXGISwapChain3), instance.put_void());

  frames.reserve(swap_chain_desc.BufferCount);

  D3D12_DESCRIPTOR_HEAP_DESC frame_heap_desc{};

  frame_heap_desc.NumDescriptors = swap_chain_desc.BufferCount;
  frame_heap_desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

  auto &device = context.device;

  device->CreateDescriptorHeap(&frame_heap_desc, __uuidof(ID3D12DescriptorHeap), frame_heap.put_void());

  const SIZE_T offset_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
  auto         heap_start  = frame_heap->GetCPUDescriptorHandleForHeapStart();

  for (auto i = 0U; i < swap_chain_desc.BufferCount; i += 1) {
    swap_chain::frame frame{};

    frame.index      = i;
    frame.swap_chain = cx::exp::ptr_ref{this}.cast < gx::swap_chain >();

    resource_reference ref;

    ref.handle.cpu_handle = heap_start;
    ref.type              = typeCPU;
    ref.created_state     = D3D12_RESOURCE_STATE_PRESENT;
    ref.format            = swap_chain_desc.Format;

    pre_instance->GetBuffer(i, __uuidof(ID3D12Resource), ref.resource.put_void());
    device->CreateRenderTargetView(ref.resource.get(), nullptr, heap_start);

    ref.resource->SetName((std::wstringstream() << L"Swap chain back buffer index " << i).str().c_str());

    frame.reference = cx::exp::ptr < gx::resource_reference, dx::resource_reference >{std::move(ref)};

    frames.emplace_back(std::move(frame));

    heap_start = D3D12_CPU_DESCRIPTOR_HANDLE{heap_start.ptr + offset_size};
  }
}


void
  swap_chain::resize(cx::size_2d const &new_size) {

  DXGI_SWAP_CHAIN_DESC1 desc{};
  instance->GetDesc1(&desc);

  needs_resize = new_size.x() != desc.Width || new_size.y() != desc.Height;
}

cx::exp::ptr_ref < swap_chain::frame >
  swap_chain::next_frame() {

  if (needs_resize) {
    needs_resize = false;

    for (auto &frame : frames) {
      auto ref = cx::exp::ptr_ref{frame->reference}.cast < dx::resource_reference >();

      ref->handle.cpu_handle = {};

      if (auto resource = ref->resource.detach()) { resource->Release(); }
    }

    const auto new_size = window->size();
    instance->ResizeBuffers(0, new_size.x(), new_size.y(), DXGI_FORMAT_UNKNOWN, 0);

    winrt::com_ptr < ID3D12Device > device;
    instance->GetDevice(__uuidof(ID3D12Device), device.put_void());

    const SIZE_T offset_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    auto         heap_start  = frame_heap->GetCPUDescriptorHandleForHeapStart();

    for (auto i = 0U; i < frames.size(); i += 1) {
      auto reference = cx::exp::ptr_ref{frames[i]->reference}.cast < dx::resource_reference >();
      instance->GetBuffer(i, __uuidof(ID3D12Resource), reference->resource.put_void());

      reference->resource->SetName((std::wstringstream() << L"Swap chain back buffer index " << i).str().c_str());

      reference->handle.cpu_handle = heap_start;
      device->CreateRenderTargetView(reference->resource.get(), nullptr, heap_start);

      heap_start = D3D12_CPU_DESCRIPTOR_HANDLE{heap_start.ptr + offset_size};
    }
  }

  return cx::exp::ptr_ref < frame >{frames[instance->GetCurrentBackBufferIndex()]};
}

void
  swap_chain::present(std::vector < ptr_ref < gx::queue > > const &dependencies) {

  assert(instance->Present(swaps_immediately ? 0 : 1, 0) == S_OK);
}

SNOW_OWL_NAMESPACE_END
