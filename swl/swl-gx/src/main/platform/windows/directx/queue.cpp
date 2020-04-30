#include "directx/queue.h"
#include "directx/render_block.h"

#include "graphics_render_block.hpp"


SNOW_OWL_NAMESPACE(gx::dx)

queue::queue   (const cx::exp::ptr_ref<dx::context>& context):
	graphics_queue(context.cast<graphics_context>()), wait(CreateEvent(nullptr, FALSE, TRUE, nullptr)), context(context) {

	D3D12_COMMAND_QUEUE_DESC queue_desc{};

	queue_desc.Flags     = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queue_desc.Type      = D3D12_COMMAND_LIST_TYPE_DIRECT;

	context->device->CreateCommandQueue(&queue_desc, __uuidof(ID3D12CommandQueue), command_queue.put_void());
	context->device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), fence.put_void());
}

queue::~queue  () = default;

cx::exp::ptr<graphics_render_block>
			queue::create_render_block  () {
	return cx::exp::ptr<graphics_render_block, dx::render_block>{ this->context };
}

void  queue::begin                (const std::vector<cx::exp::ptr_ref<graphics_queue>>& dependencies) {

	for (auto& item : dependencies) {
		auto queue = item.cast<dx::queue>();

		if (queue->fence_frame > queue->fence->GetCompletedValue()) {
			queue->fence->SetEventOnCompletion(queue->fence_frame, wait);
			WaitForSingleObject(wait, INFINITE);
		}
	}

	if (fence && fence_frame > fence->GetCompletedValue()) {
		fence->SetEventOnCompletion(fence_frame, wait);
		WaitForSingleObject(wait, INFINITE);
	}
}

void  queue::submit               (const std::vector<cx::exp::ptr_ref<graphics_render_block>>& commands) {

	if (!fence) {
		return;
	}

	std::vector<winrt::com_ptr<ID3D12GraphicsCommandList4>> commandLists{ commands.size(), nullptr };

	for (auto& command : commands) {

		auto& list = command.cast<dx::render_block>()->command_list;
		list->Close();

		commandLists.emplace_back(list);
	}

	fence_frame += 1;

	command_queue->ExecuteCommandLists(0, nullptr);

	command_queue->Signal(fence.get(), fence_frame);
}

SNOW_OWL_NAMESPACE_END
