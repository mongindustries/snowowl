#include "directx/queue.h"
#include "graphics_render_block.hpp"

SNOW_OWL_NAMESPACE(gx::dx)

queue::queue   (const cx::exp::ptr_ref<context>& context):
	graphics_queue(context.cast<graphics_context>()) {

	D3D12_COMMAND_QUEUE_DESC queue_desc{};

	queue_desc.Flags     = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queue_desc.Type      = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queue_desc.Priority  = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	
	context->device->CreateCommandQueue(&queue_desc, __uuidof(ID3D12CommandQueue), command_queue.put_void());
	context->device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), fence.put_void());
}

queue::~queue  () = default;


void queue::begin   (const std::vector<cx::exp::ptr_ref<graphics_queue>>& dependencies) {

	command_queue->Wait(fence.get(), frame);
}

void queue::submit  (const std::vector<cx::exp::ptr_ref<graphics_render_block>>& commands) {

	const uint64_t value = frame;
	frame += 1;

	command_queue->ExecuteCommandLists(0, nullptr);
	
	command_queue->Signal(fence.get(), value);
}

SNOW_OWL_NAMESPACE_END
