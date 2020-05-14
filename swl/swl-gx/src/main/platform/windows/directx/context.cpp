#include "directx/context.h"

#include <dxgidebug.h>

SNOW_OWL_NAMESPACE(gx::dx)

context::context() {

#if defined(SWL_DEBUG) || defined(DEBUG)
  winrt::com_ptr < ID3D12Debug > debug;
  D3D12GetDebugInterface(__uuidof(ID3D12Debug), debug.put_void());

  const auto flag = DXGI_CREATE_FACTORY_DEBUG;
  debug->EnableDebugLayer();
#else
  const auto flag = 0;
#endif

  CreateDXGIFactory2(flag, __uuidof(IDXGIFactory2), dxgi_factory.put_void());
  winrt::com_ptr < IDXGIAdapter > adapter{};

  uint16_t index{0};
  auto     has_adapters = dxgi_factory->EnumAdapters(index, adapter.put()) != DXGI_ERROR_NOT_FOUND;

  while (has_adapters) {
    DXGI_ADAPTER_DESC desc{};
    adapter->GetDesc(&desc);

    const auto result = D3D12CreateDevice(adapter.get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr);

    if (result == S_FALSE || result == S_OK) { break; }

    adapter.detach()->Release();
    has_adapters = dxgi_factory->EnumAdapters(++index, adapter.put()) != DXGI_ERROR_NOT_FOUND;
  }

  D3D12CreateDevice(adapter.get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), device.put_void());

#if defined(SWL_DEBUG)
  device->QueryInterface(__uuidof(ID3D12DebugDevice), debug_device.put_void());
#endif
}

SNOW_OWL_NAMESPACE_END
