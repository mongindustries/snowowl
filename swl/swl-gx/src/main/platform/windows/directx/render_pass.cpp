#include "directx/context.h"
#include "directx/render_block.h"
#include "directx/render_pass.h"
#include "directx/render_pipeline.h"

#include "directx/resource_reference.h"

SNOW_OWL_NAMESPACE(gx::dx)

render_pass::render_pass  (dx::render_block& block, std::array < pipeline::pass_output, 8 > const& context)
  : command_list(block.command_list)
  , pipeline(block.current_pipeline) {

  std::vector < D3D12_RENDER_PASS_RENDER_TARGET_DESC > rtv_desc;
  rtv_desc.reserve(context.size());

  std::vector < D3D12_RESOURCE_BARRIER > rtv_barrier_from;
  rtv_barrier_from.reserve(context.size());

  std::vector < D3D12_RESOURCE_BARRIER > rtv_barrier_to;
  rtv_barrier_to.reserve(context.size());

  for (const auto& item : context) {
    auto reference = item.output.cast < dx::resource_reference >();

    D3D12_RENDER_PASS_RENDER_TARGET_DESC rdesc{};

    switch (item.action_load) {
    case pipeline::loadOpLoad:
      rdesc.BeginningAccess.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_PRESERVE;
      break;
    case pipeline::loadOpDoNotCare:
      rdesc.BeginningAccess.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_DISCARD;
      break;
    case pipeline::loadOpClear: {
      D3D12_CLEAR_VALUE clear_value{};

      clear_value.Format = reference->format;

      clear_value.Color[0] = item.clear_color[0];
      clear_value.Color[1] = item.clear_color[1];
      clear_value.Color[2] = item.clear_color[2];
      clear_value.Color[3] = item.clear_color[3];

      D3D12_RENDER_PASS_BEGINNING_ACCESS_CLEAR_PARAMETERS clear_param{};
      clear_param.ClearValue = clear_value;

      rdesc.BeginningAccess.Clear = clear_param;
      rdesc.BeginningAccess.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR;
    } break;
    case pipeline::loadOpNoAccess:
      rdesc.BeginningAccess.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_NO_ACCESS;
      break;
    }

    switch (item.action_store) {
    case pipeline::storeOpStore:
      rdesc.EndingAccess.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;
      break;
    case pipeline::storeOpDoNotCare:
      rdesc.EndingAccess.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_DISCARD;
      break;
    case pipeline::storeOpResolve:
      rdesc.EndingAccess.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_RESOLVE;
      break;
    case pipeline::storeOpNoAccess:
      rdesc.EndingAccess.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_NO_ACCESS;
      break;
    }

    rdesc.cpuDescriptor = { reference->heap->GetCPUDescriptorHandleForHeapStart().ptr + reference->heap_offset };

    rtv_desc.emplace_back(rdesc);

    D3D12_RESOURCE_STATES state_from;
    D3D12_RESOURCE_STATES state_during;
    D3D12_RESOURCE_STATES state_to;

    switch (item.before) {
    case pipeline::transitionTypeInherit:
      state_from = reference->created_state;
      break;
    case pipeline::transitionTypeRenderTargetView:
      state_from = D3D12_RESOURCE_STATE_RENDER_TARGET;
      break;
    case pipeline::transitionTypeShaderView:
      state_from = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
      break;
    case pipeline::transitionTypeSwapChainPresent:
      state_from = D3D12_RESOURCE_STATE_PRESENT;
      break;
    case pipeline::transitionTypeCopySource:
      state_from = D3D12_RESOURCE_STATE_COPY_SOURCE;
      break;
    case pipeline::transitionTypeCopyDestination:
      state_from = D3D12_RESOURCE_STATE_COPY_DEST;
      break;
    }

    switch (item.during) {
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

    switch (item.after) {
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
    barrier_desc.Transition.StateAfter = state_during;
    barrier_desc.Transition.pResource = reference->resource.get();

    rtv_barrier_from.emplace_back(barrier_desc);

    barrier_desc.Transition.StateBefore = state_during;
    barrier_desc.Transition.StateAfter = state_to;
    barrier_desc.Transition.pResource = reference->resource.get();

    rtv_barrier_to.emplace_back(barrier_desc);
  }

  to_barriers = rtv_barrier_to;

  command_list->ResourceBarrier(rtv_barrier_from.size(), rtv_barrier_from.data());

  command_list->BeginRenderPass(rtv_desc.size(), rtv_desc.data(), nullptr, D3D12_RENDER_PASS_FLAG_NONE);
}

render_pass::~render_pass () {

  command_list->EndRenderPass();

  command_list->ResourceBarrier(to_barriers.size(), to_barriers.data());
}


void
render_pass::topology(topology_type type) {

  D3D_PRIMITIVE_TOPOLOGY dx_type = {};

  switch (type) {
  case typeTriangleList:
    dx_type = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    break;
  case typeTriganleStrip:
    dx_type = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
    break;
  case typeLineList:
    dx_type = D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
    break;
  case typeLineStrip:
    dx_type = D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP;
    break;
  case typePointList:
    dx_type = D3D10_PRIMITIVE_TOPOLOGY_POINTLIST;
    break;
  }

  command_list->IASetPrimitiveTopology(dx_type);
}

cx::exp::ptr < transition_handle >
render_pass::buffer_boundary(std::vector < std::pair < cx::exp::ptr_ref < gx::resource_reference >, buffer_transition > > const& transitions) {
  return cx::exp::ptr < transition_handle, barrier_transition >{new barrier_transition(*this, transitions)}.abstract();
}

void
render_pass::bind_buffer(render_pass_stage_binding binding, int slot, cx::exp::ptr_ref < gx::resource_reference > const& reference) {

  dx::resource_reference* ref = reference.cast<dx::resource_reference>().pointer();

  if (!pipeline) {
    return;
  }

  pipeline::shader_stage stage{ 0 };

  switch (binding) {
  case bindingGraphicsFragment:
    stage = pipeline::shader_stage::fragment;
    break;
  case bindingGraphicsVertex:
    stage = pipeline::shader_stage::vertex;
    break;
  }

  auto& slot_info = pipeline->render_inputs[stage];

  if (slot_info.bindings[slot].indirect) {
    /* TODO: specify descriptor heap in pipeline, not in buffer.
    command_list->SetGraphicsRootDescriptorTable(slot, { ref->heap->GetGPUDescriptorHandleForHeapStart().ptr + ref->heap_offset });
    */
  }
  else {
    switch (ref->resource_type) {
    case resource_type::typeSRV:
      command_list->SetGraphicsRootShaderResourceView(slot, ref->resource->GetGPUVirtualAddress() + ref->heap_offset);
      break;
    case resource_type::typeCBV:
      command_list->SetGraphicsRootConstantBufferView(slot, ref->resource->GetGPUVirtualAddress() + ref->heap_offset);
      break;
    default:
      break;
    }
  }
}

void
render_pass::bind_buffers(render_pass_stage_binding binding, std::array < cx::exp::ptr_ref < gx::resource_reference >, 16 > const& references) {

  for (auto i = 0u; i < 16; i += 1) {
    if (!references[i]) {
      continue;
    }

    bind_buffer(binding, i, references[i]);
  }
}

void
render_pass::draw(const render_pass_draw_range& vertex_range) {
  command_list->DrawInstanced(vertex_range.size, 1, vertex_range.begin, 0);
}


SNOW_OWL_NAMESPACE_END
