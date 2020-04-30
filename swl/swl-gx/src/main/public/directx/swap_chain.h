#pragma once

#include <dcomp.h>

#include <header.hpp>

#include "graphics_swap_chain.hpp"

#include "directx/context.h"
#include "directx/queue.h"

SNOW_OWL_NAMESPACE(gx::dx)

struct swap_chain: graphics_swap_chain {

	struct dx_frame: frame {

		winrt::com_ptr<ID3D12Resource> resource;
	};

	swap_chain        (const cx::exp::ptr_ref<context>& context, const cx::exp::ptr_ref<queue>& queue, const cx::exp::ptr_ref<ui::window>& window);
	
	~swap_chain       () override;

	cx::exp::ptr_ref<frame>
				next_frame  () override;

	void  present     () override;

	void  resize      (const cx::size_2d& new_size) override;


	std::atomic<uint64_t>                frame;

	HANDLE                               event_resize;


	cx::exp::ptr_ref<queue>              queue;

	winrt::com_ptr<IDXGISwapChain1>      instance;


	winrt::com_ptr<IDCompositionDevice>  comp_device;

	winrt::com_ptr<IDCompositionTarget>  comp_target;

	winrt::com_ptr<IDCompositionVisual>  comp_content;
};

SNOW_OWL_NAMESPACE_END
