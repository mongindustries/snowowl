#pragma once

#include <header.hpp>

#include <d3d12.h>

#include <dxgi.h>
#include <dxgi1_6.h>

#include <winrt/base.h>

#include "graphics_context.hpp"

SNOW_OWL_NAMESPACE(gx::dx)

struct context final : graphics_context {

	context();

	winrt::com_ptr<IDXGIFactory2>           dxgi_factory;

	winrt::com_ptr<ID3D12Device>            device;

	winrt::com_ptr<ID3D12CommandAllocator>  command_allocator;
};

SNOW_OWL_NAMESPACE_END
