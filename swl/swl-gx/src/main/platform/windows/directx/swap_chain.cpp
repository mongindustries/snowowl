#include "directx/swap_chain.h"
#include "directx/resource_reference.h"
#include "directx/render_block.h"

#include <chrono>

#include <sstream>
#include <array>

#include <window_surface.hpp>

#include <swl_window_backend.hpp>
#include <../platform/windows/swl_win32_window.hpp>

SNOW_OWL_NAMESPACE(gx::dx)

template<typename c>
using ptr_ref = cx::exp::ptr_ref<c>;

swap_chain::swap_chain  (ptr_ref<context> const& context, const cx::exp::ptr_ref<dx::queue>& present_queue, ptr_ref<ui::window> const& window) :
  graphics_swap_chain (context.cast<graphics_context>(), present_queue.cast<graphics_queue>(), window),
  event_wait          (CreateEvent(nullptr, false, false, L"Swapchain resize wait event")),
  present_queue       (present_queue),
  current_frame       (0) {

  window->swap_chain = ptr_ref<graphics_swap_chain>{ this };

  DXGI_SWAP_CHAIN_DESC1 swap_chain_desc{};

  swap_chain_desc.Width       = window->get_size().x();
  swap_chain_desc.Height      = window->get_size().y();

  swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

  swap_chain_desc.Format      = DXGI_FORMAT_B8G8R8A8_UNORM;
  swap_chain_desc.BufferCount = 3;

  swap_chain_desc.Scaling     = DXGI_SCALING_STRETCH;
  swap_chain_desc.SwapEffect  = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

  swap_chain_desc.AlphaMode   = DXGI_ALPHA_MODE_IGNORE;

  swap_chain_desc.SampleDesc.Count    = 1;
  swap_chain_desc.SampleDesc.Quality  = 0;

  const ui::window_surface surface{ window };
  auto window_handle = surface.cast<void>().pointer();

  winrt::com_ptr<IDXGISwapChain1> instance;
  // context->dxgi_factory->CreateSwapChainForComposition(present_queue->command_queue.get(), &swap_chain_desc, nullptr, instance.put());
  context->dxgi_factory->CreateSwapChainForHwnd(present_queue->command_queue.get(), (HWND) window_handle, &swap_chain_desc, nullptr, nullptr, instance.put());

  instance->QueryInterface(__uuidof(IDXGISwapChain3), this->instance.put_void());

  /*
  DCompositionCreateDevice(nullptr, __uuidof(IDCompositionDevice), comp_device.put_void());

  comp_device->CreateTargetForHwnd((HWND)window_handle, true, comp_target.put());

  comp_device->CreateVisual(comp_content.put());

  comp_content->SetContent(instance.get());

  comp_target->SetRoot(comp_content.get());
  comp_device->Commit();
  */

  frames.reserve(swap_chain_desc.BufferCount);

  D3D12_DESCRIPTOR_HEAP_DESC frameHeapDesc{};

  frameHeapDesc.NumDescriptors  = swap_chain_desc.BufferCount;
  frameHeapDesc.Type            = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

  context->device->CreateDescriptorHeap(&frameHeapDesc, __uuidof(ID3D12DescriptorHeap), frame_heap.put_void());

  SIZE_T offset_size  = context->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
  auto   heap_start   = frame_heap->GetCPUDescriptorHandleForHeapStart();

  for (auto i = 0U; i < swap_chain_desc.BufferCount; i += 1) {

    graphics_swap_chain::frame frame;

    frame.index       = i;
    frame.swap_chain  = cx::exp::ptr_ref{ this }.cast<graphics_swap_chain>();

    dx::resource_reference ref;

    ref.handle.cpu_handle = heap_start;
    ref.type              = typeCPU;
    ref.created_state     = D3D12_RESOURCE_STATE_PRESENT;
    ref.format            = swap_chain_desc.Format;

    HRESULT buffer_res = instance->GetBuffer(i, __uuidof(ID3D12Resource), ref.resource.put_void());
    context->device->CreateRenderTargetView(ref.resource.get(), nullptr, heap_start);

    ref.resource->SetName((std::wstringstream() << L"Swapchain back buffer index " << i).str().c_str());

    frame.reference       = cx::exp::ptr<graphics_resource_reference, dx::resource_reference>{ std::move(ref) };

    frames.emplace_back(std::move(frame));

    heap_start = D3D12_CPU_DESCRIPTOR_HANDLE{ heap_start.ptr + offset_size };
  }
}

swap_chain::~swap_chain () = default;


void  swap_chain::resize      (const cx::size_2d & new_size) {

  DXGI_SWAP_CHAIN_DESC1 desc{};
  instance->GetDesc1(&desc);

  needs_resize = new_size.x() != desc.Width || new_size.y() != desc.Height;
}

cx::exp::ptr_ref<graphics_swap_chain::frame>
      swap_chain::next_frame  () {

  if (needs_resize) {
    needs_resize = false;

    for (auto& frame : frames) {

      auto ref = cx::exp::ptr_ref{ frame->reference }.cast<dx::resource_reference>();

      ref->handle.cpu_handle = {};

      if (auto resource = ref->resource.detach()) {
        resource->Release();
      }
    }

    auto new_size = window->get_size();
    instance->ResizeBuffers(0, new_size.x(), new_size.y(), DXGI_FORMAT_UNKNOWN, 0);

    winrt::com_ptr<ID3D12Device> device;
    instance->GetDevice(__uuidof(ID3D12Device), device.put_void());

    SIZE_T offset_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    auto   heap_start = frame_heap->GetCPUDescriptorHandleForHeapStart();

    for (auto i = 0U; i < frames.size(); i += 1) {

      auto reference = cx::exp::ptr_ref{ frames[i]->reference }.cast<dx::resource_reference>();
      instance->GetBuffer(i, __uuidof(ID3D12Resource), reference->resource.put_void());

      reference->resource->SetName((std::wstringstream() << L"Swapchain back buffer index " << i).str().c_str());

      reference->handle.cpu_handle = heap_start;
      device->CreateRenderTargetView(reference->resource.get(), nullptr, heap_start);

      heap_start = D3D12_CPU_DESCRIPTOR_HANDLE{ heap_start.ptr + offset_size };
    }
  }

  UINT index = instance->GetCurrentBackBufferIndex();
  return cx::exp::ptr_ref<graphics_swap_chain::frame>{ frames[index] };
}

void  swap_chain::present     (std::vector<ptr_ref<graphics_queue>> const& dependencies) {

  assert(instance->Present(swaps_immediately ? 0 : 1, 0) == S_OK);
}

SNOW_OWL_NAMESPACE_END
