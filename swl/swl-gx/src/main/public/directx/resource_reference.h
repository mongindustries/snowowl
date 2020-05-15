#pragma once

#include <header.hpp>

#include <d3d12.h>
#include <winrt/base.h>

#include "resource_reference.hpp"

SNOW_OWL_NAMESPACE(gx::dx)

enum resource_reference_type {
  typeCPU,
  typeGPU,

  typeAll
};

struct resource_reference final : gx::resource_reference {

  struct {
    D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle;
    D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle;
  }                             handle;

  resource_reference_type type;
  D3D12_RESOURCE_STATES   created_state;

  DXGI_FORMAT                       format;
  winrt::com_ptr < ID3D12Resource > resource;
};

SNOW_OWL_NAMESPACE_END
