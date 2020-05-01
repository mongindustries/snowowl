#include "directx/context.h"
#include "directx/swap_chain.h"
#include "directx/queue.h"

#include <dxgidebug.h>

SNOW_OWL_NAMESPACE(gx::dx)

context::context() {

#if defined(SWL_DEBUG)
	winrt::com_ptr<ID3D12Debug> debug;
	D3D12GetDebugInterface(__uuidof(ID3D12Debug), debug.put_void());

	auto flag = DXGI_CREATE_FACTORY_DEBUG;
	debug->EnableDebugLayer();
#else
	auto flag = 0;
#endif
	
	CreateDXGIFactory2(flag, __uuidof(IDXGIFactory2), dxgi_factory.put_void());
	winrt::com_ptr<IDXGIAdapter> adapter{ };

	uint16_t index{ 0 };
	bool has_adapters = dxgi_factory->EnumAdapters(index, adapter.put()) != DXGI_ERROR_NOT_FOUND;

	while (has_adapters) {

		DXGI_ADAPTER_DESC desc{ };
		adapter->GetDesc(&desc);

		auto result = D3D12CreateDevice(adapter.get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr);

		if (result == S_FALSE) {
			break;
		}

		adapter.detach()->Release();
		has_adapters = dxgi_factory->EnumAdapters(++index, adapter.put()) != DXGI_ERROR_NOT_FOUND;
	}
	
	D3D12CreateDevice(adapter.get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), device.put_void());
}

cx::exp::ptr<graphics_swap_chain>
context::create_swap_chain(const cx::exp::ptr_ref<ui::window>& window, const cx::exp::ptr_ref<gx::graphics_queue>& present_queue) {
	return cx::exp::ptr<graphics_swap_chain, dx::swap_chain>{ new dx::swap_chain{ cx::exp::ptr_ref{ this }, present_queue.cast<dx::queue>(), window } };
}

cx::exp::ptr<graphics_queue>
context::create_queue() {
	return cx::exp::ptr<graphics_queue, dx::queue>{ new dx::queue{ cx::exp::ptr_ref{ this } } };
}

cx::exp::ptr<graphics_buffer_allocator>
context::create_allocator() {
	return cx::exp::ptr<graphics_buffer_allocator>{ nullptr };
}
SNOW_OWL_NAMESPACE_END
