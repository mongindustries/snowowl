#include "directx/queue.h"
#include "directx/render_block.h"

#include "graphics_render_block.hpp"

#include <chrono>
#include <sstream>

SNOW_OWL_NAMESPACE(gx::dx)

queue::queue(const cx::exp::ptr_ref<dx::context>& context) :
  graphics_queue(context.cast<graphics_context>()), wait(CreateEvent(nullptr, FALSE, TRUE, nullptr)) {

  D3D12_COMMAND_QUEUE_DESC queue_desc{};

  queue_desc.Flags  = D3D12_COMMAND_QUEUE_FLAG_NONE;
  queue_desc.Type   = D3D12_COMMAND_LIST_TYPE_DIRECT;

  context->device->CreateCommandQueue(&queue_desc, __uuidof(ID3D12CommandQueue), command_queue.put_void());
  context->device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), fence.put_void());

  context->device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), command_allocator.put_void());
}

queue::~queue() = default;

cx::exp::ptr<graphics_render_block>
queue::create_render_block(const cx::exp::ptr_ref<graphics_render_pipeline> & pipeline) {
  return cx::exp::ptr<graphics_render_block, dx::render_block>{ new dx::render_block{ cx::exp::ptr_ref{ this }, pipeline } };
}

void  queue::begin(const std::vector<cx::exp::ptr_ref<graphics_queue>> & dependencies) {

  auto time = std::chrono::high_resolution_clock::now();

  command_queue->Signal(fence.get(), fence_frame);
  fence_frame += 1;

  if (fence && fence_frame > fence->GetCompletedValue()) {

    fence->SetEventOnCompletion(fence_frame - 1, wait); // waiting for previous frame to complete
    WaitForSingleObject(wait, INFINITE);
  }

  auto after      = std::chrono::high_resolution_clock::now();
  auto difference = std::chrono::duration_cast<std::chrono::milliseconds>(after - time);

  command_allocator->Reset();
}

void  queue::submit(const std::vector<cx::exp::ptr_ref<graphics_render_block>> & commands) {

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
