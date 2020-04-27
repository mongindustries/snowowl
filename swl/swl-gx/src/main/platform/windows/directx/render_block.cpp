#include "directx/render_block.h"

SNOW_OWL_NAMESPACE(gx::dx)

render_block::render_block(const cx::exp::ptr_ref<context>& context): graphics_render_block() {
	context->device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, context->command_allocator.get(), nullptr, __uuidof(ID3D12GraphicsCommandList4), command_list.put_void());
}

SNOW_OWL_NAMESPACE_END
