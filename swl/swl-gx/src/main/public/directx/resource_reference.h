#pragma once

#include <header.hpp>

#include <d3d12.h>
#include <winrt/base.h>

#include "resource_reference.hpp"

SNOW_OWL_NAMESPACE(gx::dx)

enum resource_type {
  typeCBV,
  typeSRV,
  typeRTV,
  typeUAV
};

struct resource_reference final : gx::resource_reference {

  resource_type           resource_type;
  D3D12_RESOURCE_STATES   created_state;

  DXGI_FORMAT format;

  winrt::com_ptr < ID3D12Resource > resource;

  winrt::com_ptr < ID3D12DescriptorHeap > heap;
  size_t                                  heap_offset{0};
};

SNOW_OWL_NAMESPACE_END
