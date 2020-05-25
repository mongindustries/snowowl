#include "directx/render_block.h"
#include "directx/resource_reference.h"
#include "directx/queue.h"

#include "render_pass.hpp"

SNOW_OWL_NAMESPACE(gx::dx)

struct barrier_transition final : transition_handle {

  std::vector < D3D12_RESOURCE_BARRIER >       barriers_out;
  winrt::com_ptr < ID3D12GraphicsCommandList > command_list;

  barrier_transition(render_pass &pass, std::vector < std::pair < cx::exp::ptr_ref < gx::resource_reference >, buffer_transition > > const &transitions) {

    command_list = pass.command_list;

    std::vector < D3D12_RESOURCE_BARRIER > barriers_in;

    barriers_in.reserve(transitions.size());
    barriers_out.reserve(transitions.size());

    for (auto &item : transitions) {
      const auto ref = get < 0 >(item).cast < resource_reference >();

      D3D12_RESOURCE_STATES from_state{};
      D3D12_RESOURCE_STATES to_state{};

      auto const &transition = get < 1 >(item);

      switch (transition.during) {
      case buffer_transition::transitionInherit:
        from_state = ref->created_state;
        break;
      case buffer_transition::transitionShaderView:
        from_state = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
        break;
      case buffer_transition::transitionRenderTargetView:
        from_state = D3D12_RESOURCE_STATE_RENDER_TARGET;
        break;
      case buffer_transition::transitionConstantView:
        from_state = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER | D3D12_RESOURCE_STATE_INDEX_BUFFER;
      }

      switch (transition.after) {
      case buffer_transition::transitionInherit:
        to_state = ref->created_state;
        break;
      case buffer_transition::transitionShaderView:
        to_state = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
        break;
      case buffer_transition::transitionRenderTargetView:
        to_state = D3D12_RESOURCE_STATE_RENDER_TARGET;
        break;
      case buffer_transition::transitionConstantView:
        to_state = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER | D3D12_RESOURCE_STATE_INDEX_BUFFER;
      }

      assert(transition.before == ref->created_state || transition.before == gx::transitionInherit);

      D3D12_RESOURCE_BARRIER barrier_desc{};

      barrier_desc.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
      barrier_desc.Transition.pResource   = ref->resource.get();
      barrier_desc.Transition.StateBefore = ref->created_state;
      barrier_desc.Transition.StateAfter  = from_state;

      if (ref->created_state != from_state) { barriers_in.emplace_back(barrier_desc); }

      barrier_desc.Transition.StateBefore = from_state;
      barrier_desc.Transition.StateAfter  = to_state;

      if (from_state != to_state) { barriers_out.emplace_back(barrier_desc); }
    }

    if (!barriers_in.empty())
      pass.command_list->ResourceBarrier(barriers_in.size(), barriers_in.data());
  }

  ~barrier_transition() override = default;

  void
    release() override {
    if (!barriers_out.empty())
      command_list->ResourceBarrier(barriers_out.size(), barriers_out.data());
  }
};

render_block::render_block(dx::queue &queue, dx::render_pipeline *pipeline)
  : gx::render_block(queue, pipeline)
  , allocator(queue.command_allocator) {

  winrt::com_ptr < ID3D12Device > device;
  queue.command_queue->GetDevice(__uuidof(ID3D12Device), device.put_void());

  device->CreateCommandList(0,
                            D3D12_COMMAND_LIST_TYPE_DIRECT, allocator.get(), nullptr,
                            __uuidof(ID3D12GraphicsCommandList4), command_list.put_void());

  if (pipeline != nullptr) {
    command_list->SetPipelineState(pipeline->pipeline_state.get());
    command_list->SetGraphicsRootSignature(pipeline->root_signature.get());

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

  command_list->SetPipelineState(dx_pipeline.pipeline_state.get());
  command_list->SetGraphicsRootSignature(dx_pipeline.root_signature.get());
}

SNOW_OWL_NAMESPACE_END
