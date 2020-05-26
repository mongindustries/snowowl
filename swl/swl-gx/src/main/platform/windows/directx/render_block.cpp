#include "directx/render_block.h"
#include "directx/render_pass.h"
#include "directx/resource_reference.h"
#include "directx/queue.h"

#include "render_pass.hpp"

SNOW_OWL_NAMESPACE(gx::dx)

render_block::render_block(dx::queue &queue, dx::render_pipeline *pipeline)
  : gx::render_block(queue, pipeline)
  , allocator(queue.command_allocator) {

  winrt::com_ptr < ID3D12Device > device;
  queue.command_queue->GetDevice(__uuidof(ID3D12Device), device.put_void());

  device->CreateCommandList(0,
                            D3D12_COMMAND_LIST_TYPE_DIRECT, allocator.get(), nullptr,
                            __uuidof(ID3D12GraphicsCommandList4), command_list.put_void());

  if (pipeline != nullptr) {
    command_list->SetPipelineState        (pipeline->pipeline_state.get());
    command_list->SetGraphicsRootSignature(pipeline->root_signature.get());

    if (pipeline->descriptor_buf) {
      std::array<ID3D12DescriptorHeap*, 1> heaps { pipeline->descriptor_buf.get() };
      command_list->SetDescriptorHeaps(heaps.size(), heaps.data());
    }

    current_pipeline = cx::exp::ptr_ref<dx::render_pipeline>{ pipeline };
  }

  command_list->Close();
}

void
  render_block::close() { command_list->Close(); }

void
  render_block::reset(gx::render_pipeline &pipeline) {
  command_list->Reset(allocator.get(), nullptr);

  auto &dx_pipeline = dynamic_cast<dx::render_pipeline&>(pipeline);

  current_pipeline = cx::exp::ptr_ref{ &dx_pipeline };

  command_list->SetPipelineState          (dx_pipeline.pipeline_state.get());
  command_list->SetGraphicsRootSignature  (dx_pipeline.root_signature.get());

  if (dx_pipeline.descriptor_buf) {
    std::array<ID3D12DescriptorHeap*, 1> heaps{ dx_pipeline.descriptor_buf.get() };
    command_list->SetDescriptorHeaps(heaps.size(), heaps.data());
  }
}

SNOW_OWL_NAMESPACE_END
