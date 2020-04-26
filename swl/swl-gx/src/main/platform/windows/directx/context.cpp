#include "directx/context.h"

SNOW_OWL_NAMESPACE(gx::dx)

context::context() {

	CreateDXGIFactory(__uuidof(IDXGIFactory2), dxgi_factory.put_void());

	// dxgi_factory->CreateSwapChainForComposition()

	winrt::com_ptr<IDXGIAdapter> adapter{};

	uint16_t index{ 0 };
	bool has_adapters = dxgi_factory->EnumAdapters(index, adapter.put()) != DXGI_ERROR_NOT_FOUND;

	while (has_adapters) {

		if (D3D12CreateDevice(reinterpret_cast<IUnknown*>(adapter.get()), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) {
			break;
		}

		has_adapters = dxgi_factory->EnumAdapters(++index, adapter.put()) != DXGI_ERROR_NOT_FOUND;
	}
	
	D3D12CreateDevice(reinterpret_cast<IUnknown*>(adapter.get()), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), device.put_void());

	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), command_allocator.put_void());
}

SNOW_OWL_NAMESPACE_END
