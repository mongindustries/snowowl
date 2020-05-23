#include "directx/queue.h"
#include "directx/render_block.h"

#include "render_block.hpp"

#include "tell.hpp"
#include "directx/buffer.h"

struct upload_desc {
  winrt::com_ptr < ID3D12Resource > source;
  winrt::com_ptr < ID3D12Resource > dest;

  size_t size;
};

SNOW_OWL_NAMESPACE(gx::dx)

queue::queue(dx::context &context)
  : gx::queue(context)
  , fence_frame(0)
  , wait(CreateEvent(nullptr, FALSE, TRUE, nullptr)) {

  D3D12_COMMAND_QUEUE_DESC queue_desc{};

  queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
  queue_desc.Type  = D3D12_COMMAND_LIST_TYPE_DIRECT;

  auto &device = context.device;

  device->CreateCommandQueue(&queue_desc, __uuidof(ID3D12CommandQueue), command_queue.put_void());

  device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), command_allocator.put_void());
  device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), transfer_allocator.put_void());

  device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), fence.put_void());

  device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, transfer_allocator.get(), nullptr, __uuidof(ID3D12GraphicsCommandList), command_list_transfer.put_void());
  command_list_transfer->Close();
}

queue::~queue() {

  begin({ }); // one last dance
}

void
  queue::begin(std::vector < std::reference_wrapper < gx::queue > > const &dependencies) {

  command_queue->Signal(fence.get(), fence_frame);
  fence_frame += 1;

  if (fence && fence_frame > fence->GetCompletedValue()) {
    fence->SetEventOnCompletion(fence_frame - 1, wait); // waiting for previous frame to complete
    WaitForSingleObject(wait, INFINITE);
  }

  command_allocator->Reset();
}

void
  queue::submit(std::vector < std::reference_wrapper < gx::render_block > > const &commands) {

  if (!fence) { return; }

  std::vector < ID3D12CommandList * > command_lists;
  command_lists.reserve(commands.size());

  for (const auto &command : commands) { command_lists.emplace_back(dynamic_cast < render_block & >(command.get()).command_list.get()); }

  command_queue->ExecuteCommandLists(command_lists.size(), command_lists.data());
}

void
  queue::transfer(std::vector < cx::exp::ptr_ref < buffer_staging > > const &buffers) {

  if (!fence) { return; }

  transfer_allocator->Reset();
  command_list_transfer->Reset(transfer_allocator.get(), nullptr);

  std::vector < D3D12_RESOURCE_BARRIER > barriers_in;
  std::vector < D3D12_RESOURCE_BARRIER > barriers_out;

  barriers_in.reserve(buffers.size());
  barriers_out.reserve(buffers.size());

  std::vector < upload_desc > upload_descs;

  upload_descs.reserve(buffers.size());

  for (const auto &buffer : buffers) {
    try {
      auto &item = dynamic_cast < buffer_data_staging & >(buffer.get());

      item.ref->data_initialized = true;

      upload_descs.emplace_back(upload_desc{
                                    item.buffer_staging,
                                    item.buffer,
                                    item.size
                                });

      auto barrier_in = cx::tell < D3D12_RESOURCE_BARRIER >({}, [&item](D3D12_RESOURCE_BARRIER &object) {
        object.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        object.Transition.Subresource = 0;
        object.Transition.pResource   = item.buffer.get();

        object.Transition.StateAfter  = D3D12_RESOURCE_STATE_COPY_DEST;
        object.Transition.StateBefore = item.source_state;
      });

      auto barrier_out = cx::tell < D3D12_RESOURCE_BARRIER >({}, [&item](D3D12_RESOURCE_BARRIER &object) {
        object.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        object.Transition.Subresource = 0;
        object.Transition.pResource   = item.buffer.get();

        object.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
        object.Transition.StateAfter  = item.target_state;
      });

      if (barrier_in.Transition.StateBefore != barrier_in.Transition.StateAfter) { barriers_in.emplace_back(barrier_in); }

      if (barrier_out.Transition.StateBefore != barrier_out.Transition.StateAfter) { barriers_out.emplace_back(barrier_out); }

    } catch (std::bad_cast const &) { }
  }

  if (!barriers_in.empty())
    command_list_transfer->ResourceBarrier(barriers_in.size(), barriers_in.data());

  for (const auto &desc : upload_descs) {
    command_list_transfer->CopyBufferRegion(desc.dest.get(), 0, desc.source.get(), 0, desc.size);
  }

  command_list_transfer->ResourceBarrier(barriers_out.size(), barriers_out.data());

  command_list_transfer->Close();

  std::array < ID3D12CommandList *, 1 > blocks{
      command_list_transfer.get()
  };

  command_queue->ExecuteCommandLists(blocks.size(), blocks.data());
}

SNOW_OWL_NAMESPACE_END
