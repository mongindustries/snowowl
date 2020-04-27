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


	cx::exp::ptr<graphics_swap_chain>
		create_swap_chain  (const cx::exp::ptr_ref<ui::window>& window, const cx::exp::ptr_ref<gx::graphics_queue>& present_queue) override;

	cx::exp::ptr<graphics_queue>
		create_queue       () override;

	cx::exp::ptr<graphics_buffer_allocator>
		create_allocator   () override;


	winrt::com_ptr<IDXGIFactory2>           dxgi_factory;

	winrt::com_ptr<ID3D12Device>            device;

	winrt::com_ptr<ID3D12CommandAllocator>  command_allocator;
};

SNOW_OWL_NAMESPACE_END
