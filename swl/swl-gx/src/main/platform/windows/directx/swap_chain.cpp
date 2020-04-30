#include "directx/swap_chain.h"

#include <sstream>

#include <window_surface.hpp>

#include <swl_window_backend.hpp>
#include <../platform/windows/swl_win32_window.hpp>

SNOW_OWL_NAMESPACE(gx::dx)

swap_chain::swap_chain        (const cx::exp::ptr_ref<context>& context, const cx::exp::ptr_ref<dx::queue>& queue, const cx::exp::ptr_ref<ui::window>& window):
	graphics_swap_chain(context.cast<graphics_context>(), queue.cast<graphics_queue>(), window), queue(queue), event_resize(nullptr) {
	
	window->swap_chain = cx::exp::ptr_ref<graphics_swap_chain>{ this };

	DXGI_SWAP_CHAIN_DESC1 swap_chain_desc{};

	swap_chain_desc.Width       = window->get_size().x();
	swap_chain_desc.Height      = window->get_size().y();

	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swap_chain_desc.Format      = DXGI_FORMAT_B8G8R8A8_UNORM;
	swap_chain_desc.BufferCount = 3;

	swap_chain_desc.SampleDesc.Count    = 1;
	swap_chain_desc.SampleDesc.Quality  = 0;

	swap_chain_desc.Scaling     = DXGI_SCALING_STRETCH;
	swap_chain_desc.SwapEffect  = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

	swap_chain_desc.AlphaMode   = DXGI_ALPHA_MODE_IGNORE;

	context->dxgi_factory->CreateSwapChainForComposition(queue->command_queue.get(), &swap_chain_desc, nullptr, instance.put());

	const ui::window_surface surface{ window };
	auto window_handle = surface.cast<void>().pointer();

	DCompositionCreateDevice(nullptr, __uuidof(IDCompositionDevice), comp_device.put_void());

	SetWindowLongPtr((HWND) window_handle, GWL_EXSTYLE, WS_EX_NOREDIRECTIONBITMAP);
	comp_device->CreateTargetForHwnd((HWND) window_handle, true, comp_target.put());

	comp_device->CreateVisual(comp_content.put());

	comp_content->SetContent(instance.get());

	comp_target->SetRoot(comp_content.get());
	comp_device->Commit();

	frames.reserve(swap_chain_desc.BufferCount);

	for(auto i = 0U; i < swap_chain_desc.BufferCount; i += 1) {

		dx_frame frame;

		frame.index       = i;
		frame.swap_chain  = cx::exp::ptr_ref{ this }.cast<graphics_swap_chain>();

		HRESULT buf = instance->GetBuffer(i, __uuidof(ID3D12Resource), frame.resource.put_void());

		auto info = (std::wstringstream() << L"Swap Chain buffer " << frame.index).str();
		frame.resource->SetName(info.c_str());

		cx::exp::ptr<graphics_swap_chain::frame, dx_frame> obj{ std::move(frame) };
		frames.emplace_back(obj.abstract_and_release());
	}
}

swap_chain::~swap_chain       () = default;

void  swap_chain::resize      (const cx::size_2d& new_size) {
	
	for (auto& frame : frames) {
		auto dx_frame = cx::exp::ptr_ref<graphics_swap_chain::frame>{ frame }.cast<swap_chain::dx_frame>();

		if (auto resource = dx_frame->resource.detach()) {
			resource->Release();
		}
	}

	// wait for current queue execution to complete.
	// TODO: check queue status instead, and assert if begin is not called (as begin will also do the same thing).
	if (queue->fence_frame > queue->fence->GetCompletedValue()) {
		queue->fence->SetEventOnCompletion(queue->fence_frame, event_resize);
		WaitForSingleObject(event_resize, INFINITE);
	}

	instance->ResizeBuffers(0, new_size.x(), new_size.y(), DXGI_FORMAT_UNKNOWN, 0);

	for (auto& frame : frames) {
		auto dx_frame = cx::exp::ptr_ref<graphics_swap_chain::frame>{ frame }.cast<swap_chain::dx_frame>();
		instance->GetBuffer(dx_frame->index, __uuidof(ID3D12Resource), dx_frame->resource.put_void());

		auto info = (std::wstringstream() << L"Swap Chain buffer " << dx_frame->index).str();
		dx_frame->resource->SetName(info.c_str());
	}

	frame = 0;
}

cx::exp::ptr_ref<graphics_swap_chain::frame>
			swap_chain::next_frame  () {
	return cx::exp::ptr_ref<graphics_swap_chain::frame>{ frames[frame % frames.size()] };
}

void  swap_chain::present     () {

	if (!this->should_present) {

		OutputDebugString(L"Frame skipped!\n");
		return;
	}

	instance->Present(swaps_immediately ? 0 : 1, 0);

	frame += 1;
}

SNOW_OWL_NAMESPACE_END
