#include "directx/swap_chain.h"

#include <window_surface.hpp>

SNOW_OWL_NAMESPACE(gx::dx)

swap_chain::swap_chain        (const cx::exp::ptr_ref<context>& context, const cx::exp::ptr_ref<dx::queue>& queue, const cx::exp::ptr_ref<ui::window>& window):
	graphics_swap_chain(context.cast<graphics_context>(), queue.cast<graphics_queue>(), window), queue(queue) {
	
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

	comp_device->CreateTargetForHwnd((HWND) window_handle, true, comp_target.put());

	comp_device->CreateVisual(comp_content.put());

	comp_content->SetContent(instance.get());

	comp_target->SetRoot(comp_content.get());
	comp_device->Commit();

	frames.reserve(swap_chain_desc.BufferCount);

	needs_resize = false;
	cur_size = window->get_size();
	
	for(auto i = 0U; i < swap_chain_desc.BufferCount; i += 1) {

		dx_frame frame;

		frame.index       = i;
		frame.swap_chain  = cx::exp::ptr_ref{ this }.cast<graphics_swap_chain>();

		HRESULT buf = instance->GetBuffer(i, __uuidof(ID3D12Resource), frame.resource.put_void());

		cx::exp::ptr<graphics_swap_chain::frame, dx_frame> obj{ std::move(frame) };
		frames.emplace_back(obj.abstract_and_release());
	}

	window->event_on_resize.emplace_back([&](const ui::window&, const cx::rect& rect) {

		needs_resize = cur_size.components == rect.size.components;
		cur_size = rect.size;
	});
}

swap_chain::~swap_chain       () = default;

cx::exp::ptr_ref<graphics_swap_chain::frame>
			swap_chain::next_frame  () {

	if (needs_resize) {
		needs_resize = false;

		for (auto& item : frames) {
			auto frame = cx::exp::ptr_ref<graphics_swap_chain::frame>{ item }.cast<dx_frame>();
			frame->resource = nullptr;
		}

		instance->ResizeBuffers(0, cur_size.x(), cur_size.y(), DXGI_FORMAT_UNKNOWN, 0);

		for (auto& item : frames) {
			auto frame = cx::exp::ptr_ref<graphics_swap_chain::frame>{ item }.cast<dx_frame>();

			instance->GetBuffer(item->index, __uuidof(ID3D12Resource), frame->resource.put_void());
		}
	}

	return cx::exp::ptr_ref<graphics_swap_chain::frame>{ frames[frame % frames.size()] };
}

void  swap_chain::present     () {

	instance->Present(1, 0);

	frame += 1;
}

SNOW_OWL_NAMESPACE_END
