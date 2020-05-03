#include "directx/queue.h"
#include "directx/render_block.h"

#include "render_block.hpp"

#include <chrono>
#include <sstream>

SNOW_OWL_NAMESPACE(gx::dx)

queue::queue        (dx::context& context) : gx::queue(context), fence_frame(0), wait(CreateEvent(nullptr, FALSE, TRUE, nullptr)) {

  D3D12_COMMAND_QUEUE_DESC queue_desc{};

  queue_desc.Flags  = D3D12_COMMAND_QUEUE_FLAG_NONE;
  queue_desc.Type   = D3D12_COMMAND_LIST_TYPE_DIRECT;

  auto& device      = context.device;

  device->CreateCommandQueue(&queue_desc, __uuidof(ID3D12CommandQueue), command_queue.put_void());
  device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), command_allocator.put_void());

  device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), fence.put_void());
}


void  queue::begin  (std::vector<cx::exp::ptr_ref<gx::queue>> const& dependencies) {

  command_queue->Signal(fence.get(), fence_frame);
  fence_frame += 1;

  if (fence && fence_frame > fence->GetCompletedValue()) {

    fence->SetEventOnCompletion(fence_frame - 1, wait); // waiting for previous frame to complete
    WaitForSingleObject(wait, INFINITE);
  }

  command_allocator->Reset();
}

void  queue::submit (std::vector<cx::exp::ptr_ref<gx::render_block>> const& commands) {

  if (!fence) {
    return;
  }

  std::vector<ID3D12CommandList*> commandLists;
  commandLists.reserve(commands.size());

  for (auto& command : commands) {
    commandLists.emplace_back(command.cast<dx::render_block>()->command_list.get());
  }

  command_queue->ExecuteCommandLists(commandLists.size(), commandLists.data());
}

SNOW_OWL_NAMESPACE_END
