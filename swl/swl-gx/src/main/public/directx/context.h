#pragma once

#include <header.hpp>

#include <d3d12.h>
#include <dxgi1_6.h>

#include <winrt/base.h>

#include "context.hpp"

SNOW_OWL_NAMESPACE(gx::dx)

struct context final : gx::context {

  context();


  winrt::com_ptr < IDXGIFactory2 > dxgi_factory;

  winrt::com_ptr < ID3D12Device > device;

#if defined(SWL_DEBUG)
  winrt::com_ptr < ID3D12DebugDevice > debug_device;
#endif
};

SNOW_OWL_NAMESPACE_END
