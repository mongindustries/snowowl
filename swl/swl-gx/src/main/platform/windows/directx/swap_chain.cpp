#include "directx/swap_chain.h"

#include <window_surface.hpp>

SNOW_OWL_NAMESPACE(gx::dx)

swap_chain::swap_chain        (const cx::exp::ptr_ref<context>& context, const cx::exp::ptr_ref<dx::queue>& queue, const cx::exp::ptr_ref<ui::window>& window):
	graphics_swap_chain(context.cast<graphics_context>(), queue.cast<graphics_queue>(), window), queue(queue) {
	
	DXGI_SWAP_CHAIN_DESC1 swap_chain_desc{};

	swap_chain_desc.Width       = window->get_size().x();
	swap_chain_desc.Height      = window->get_size().y();

	swap_chain_desc.Format      = DXGI_FORMAT_B8G8R8A8_UNORM;
	swap_chain_desc.BufferCount = 3;

	swap_chain_desc.SampleDesc.Count    = 1;
	swap_chain_desc.SampleDesc.Quality  = 0;

	swap_chain_desc.Scaling     = DXGI_SCALING_STRETCH;
	swap_chain_desc.SwapEffect  = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

	swap_chain_desc.AlphaMode   = DXGI_ALPHA_MODE_PREMULTIPLIED;

	context->dxgi_factory->CreateSwapChainForComposition(queue->command_queue.get(), &swap_chain_desc, nullptr, instance.put());

	const ui::window_surface surface{ window };
	const HWND window_handle = surface.cast<HWND>();

	DCompositionCreateDevice(nullptr, __uuidof(IDCompositionDevice), comp_device.put_void());

	comp_device->CreateTargetForHwnd(window_handle, true, comp_target.put());

	comp_device->CreateVisual(comp_content.put());

	comp_content->SetContent(instance.get());

	comp_target->SetRoot(comp_content.get());
	comp_device->Commit();

	frames.reserve(swap_chain_desc.BufferCount);
	
	for(auto i = 0U; i < swap_chain_desc.BufferCount; i += 1) {

		dx_frame frame;

		frame.index       = i;
		frame.swap_chain  = cx::exp::ptr_ref{ this }.cast<graphics_swap_chain>();

		instance->GetBuffer(i, __uuidof(ID3D12Resource), frame.resource.put_void());

		cx::exp::ptr<graphics_swap_chain::frame, dx_frame> obj{ std::move(frame) };
		frames.emplace_back(obj.abstract_and_release());
	}

	std::function<void(const ui::window&, const cx::rect&)> func = [&](const ui::window&, const cx::rect& rect) {

		for (auto& item: frames) {
			auto frame = cx::exp::ptr_ref<graphics_swap_chain::frame>{ item }.cast<dx_frame>();
			frame->resource->Release();
			frame->resource = nullptr;
		}
		
		instance->ResizeBuffers(0, rect.size.x(), rect.size.y(), DXGI_FORMAT_UNKNOWN, 0);

		for (auto& item : frames) {
			auto frame = cx::exp::ptr_ref<graphics_swap_chain::frame>{ item }.cast<dx_frame>();

			instance->GetBuffer(item->index, __uuidof(ID3D12Resource), frame->resource.put_void());
		}
	};
	
	window->event_on_resize.emplace_back(func);
}

swap_chain::~swap_chain       () = default;

cx::exp::ptr_ref<graphics_swap_chain::frame>
			swap_chain::next_frame  () {

	const auto ref = cx::exp::ptr_ref<graphics_swap_chain::frame>{ frames[frame] };

	frame = ++frame % frames.size();
	
	return ref;
}

void  swap_chain::present     () {

	instance->Present1(1, 0, nullptr);
}

SNOW_OWL_NAMESPACE_END
