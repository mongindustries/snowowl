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

  command_list->Close();
}

void
  render_block::close() { command_list->Close(); }

void
  render_block::reset(gx::render_pipeline &pipeline) { command_list->Reset(allocator.get(), nullptr); }


render_pass::render_pass(dx::render_block &block, std::vector < gx::render_pass_context > const &context)
  : command_list(block.command_list) {

  std::vector < D3D12_RENDER_PASS_RENDER_TARGET_DESC > rtv_desc;
  rtv_desc.reserve(context.size());

  std::vector < D3D12_RESOURCE_BARRIER > rtv_barrier_from;
  rtv_barrier_from.reserve(context.size());

  std::vector < D3D12_RESOURCE_BARRIER > rtv_barrier_to;
  rtv_barrier_to.reserve(context.size());

  for (const auto &item : context) {
    auto reference = item.reference.cast < dx::resource_reference >();

    D3D12_RENDER_PASS_RENDER_TARGET_DESC rdesc{};

    switch (item.action_load) {
    case loadOpLoad:
      rdesc.BeginningAccess.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_PRESERVE;
      break;
    case loadOpDoNotCare:
      rdesc.BeginningAccess.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_DISCARD;
      break;
    case loadOpClear: {
      D3D12_CLEAR_VALUE clear_value{};

      clear_value.Format = reference->format;

      clear_value.Color[0] = item.load_clear[0];
      clear_value.Color[1] = item.load_clear[1];
      clear_value.Color[2] = item.load_clear[2];
      clear_value.Color[3] = item.load_clear[3];

      D3D12_RENDER_PASS_BEGINNING_ACCESS_CLEAR_PARAMETERS clear_param{};
      clear_param.ClearValue = clear_value;

      rdesc.BeginningAccess.Clear = clear_param;
      rdesc.BeginningAccess.Type  = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR;
    }
    break;
    case loadOpNoAccess:
      rdesc.BeginningAccess.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_NO_ACCESS;
      break;
    }

    switch (item.action_store) {
    case storeOpStore:
      rdesc.EndingAccess.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;
      break;
    case storeOpDoNotCare:
      rdesc.EndingAccess.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_DISCARD;
      break;
    case storeOpResolve:
      rdesc.EndingAccess.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_RESOLVE;
      break;
    case storeOpNoAccess:
      rdesc.EndingAccess.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_NO_ACCESS;
      break;
    }

    rdesc.cpuDescriptor = reference->handle.cpu_handle;

    rtv_desc.emplace_back(rdesc);

    D3D12_RESOURCE_STATES state_from;
    D3D12_RESOURCE_STATES state_during;
    D3D12_RESOURCE_STATES state_to;

    switch (item.transition_before) {
    case transitionInherit:
      state_from = reference->created_state;
      break;
    case transitionRenderTargetView:
      state_from = D3D12_RESOURCE_STATE_RENDER_TARGET;
      break;
    case transitionShaderResource:
      state_from = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
      break;
    case transitionSwapChainPresent:
      state_from = D3D12_RESOURCE_STATE_PRESENT;
      break;
    case transitionCopySource:
      state_from = D3D12_RESOURCE_STATE_COPY_SOURCE;
      break;
    case transitionCopyDestination:
      state_from = D3D12_RESOURCE_STATE_COPY_DEST;
      break;
    }

    switch (item.transition_during) {
    case transitionInherit:
      state_during = reference->created_state;
      break;
    case transitionRenderTargetView:
      state_during = D3D12_RESOURCE_STATE_RENDER_TARGET;
      break;
    case transitionShaderResource:
      state_during = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
      break;
    case transitionSwapChainPresent:
      state_during = D3D12_RESOURCE_STATE_PRESENT;
      break;
    case transitionCopySource:
      state_during = D3D12_RESOURCE_STATE_COPY_SOURCE;
      break;
    case transitionCopyDestination:
      state_during = D3D12_RESOURCE_STATE_COPY_DEST;
      break;
    }

    switch (item.transition_after) {
    case transitionInherit:
      state_to = reference->created_state;
      break;
    case transitionRenderTargetView:
      state_to = D3D12_RESOURCE_STATE_RENDER_TARGET;
      break;
    case transitionShaderResource:
      state_to = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
      break;
    case transitionSwapChainPresent:
      state_to = D3D12_RESOURCE_STATE_PRESENT;
      break;
    case transitionCopySource:
      state_to = D3D12_RESOURCE_STATE_COPY_SOURCE;
      break;
    case transitionCopyDestination:
      state_to = D3D12_RESOURCE_STATE_COPY_DEST;
      break;
    }

    D3D12_RESOURCE_BARRIER barrier_desc{};

    barrier_desc.Transition.StateBefore = state_from;
    barrier_desc.Transition.StateAfter  = state_during;
    barrier_desc.Transition.pResource   = reference->resource.get();

    rtv_barrier_from.emplace_back(barrier_desc);

    barrier_desc.Transition.StateBefore = state_during;
    barrier_desc.Transition.StateAfter  = state_to;
    barrier_desc.Transition.pResource   = reference->resource.get();

    rtv_barrier_to.emplace_back(barrier_desc);
  }

  to_barriers = rtv_barrier_to;

  command_list->ResourceBarrier(rtv_barrier_from.size(), rtv_barrier_from.data());

  command_list->BeginRenderPass(rtv_desc.size(), rtv_desc.data(), nullptr, D3D12_RENDER_PASS_FLAG_NONE);

  /*
  command_list->SetPipelineState(nullptr);
  command_list->SetGraphicsRootSignature(nullptr);*/
}

render_pass::~render_pass() {

  command_list->EndRenderPass();

  command_list->ResourceBarrier(to_barriers.size(), to_barriers.data());
}

void
  render_pass::set_viewport(const cx::size_2d &value) {}

void
  render_pass::set_scissor(const cx::rect &value) {}

void
  render_pass::set_topology(topology_type type) {}

cx::exp::ptr < transition_handle >
  render_pass::buffer_boundary(
    std::vector < std::pair < cx::exp::ptr_ref < gx::resource_reference >, buffer_transition > > const &transitions) { return cx::exp::ptr < transition_handle, barrier_transition >{new barrier_transition(*this, transitions)}.abstract(); }

void
  render_pass::bind_buffer(render_pass_stage_binding binding, int slot, cx::exp::ptr_ref < gx::resource_reference > const &reference) {}

void
  render_pass::bind_buffers(render_pass_stage_binding binding, std::array < cx::exp::ptr_ref < gx::resource_reference >, 16 > const &references) { }

void
  render_pass::draw(const render_pass_draw_range &vertex_range) {}

SNOW_OWL_NAMESPACE_END
