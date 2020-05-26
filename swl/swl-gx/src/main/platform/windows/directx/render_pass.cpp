#include "directx/context.h"
#include "directx/render_block.h"
#include "directx/render_pass.h"
#include "directx/render_pipeline.h"

#include "directx/resource_reference.h"

SNOW_OWL_NAMESPACE(gx::dx)

struct barrier_transition final : render_pass_resource_scope {

  std::vector < D3D12_RESOURCE_BARRIER >       barriers_out;
  winrt::com_ptr < ID3D12GraphicsCommandList > command_list;

  barrier_transition  (render_pass &pass, std::array< pipeline::pass_input, NBS> const &transitions) {

    command_list = pass.command_list;

    std::vector < D3D12_RESOURCE_BARRIER > barriers_in;

    barriers_in .reserve(transitions.size());
    barriers_out.reserve(transitions.size());

    for (const auto &item : transitions) {
      const auto ref = item.input.cast < resource_reference >();

      if(!ref) {
        continue;
      }

      D3D12_RESOURCE_STATES from_state;
      D3D12_RESOURCE_STATES to_state;

      switch (item.during) {
      case pipeline::transitionTypeInherit:
        from_state = ref->created_state;
        break;
      case pipeline::transitionTypeRenderTargetView:
        from_state = D3D12_RESOURCE_STATE_RENDER_TARGET;
        break;
      case pipeline::transitionTypeConstantView:
        from_state = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
        break;
      default:
        switch (item.stage.value) {
        case 0:
          from_state = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
          break;
        default:
          from_state = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
          break;
        }
        break;
      }

      switch (item.after) {
      case pipeline::transitionTypeInherit:
        to_state = ref->created_state;
        break;
      case pipeline::transitionTypeRenderTargetView:
        to_state = D3D12_RESOURCE_STATE_RENDER_TARGET;
        break;
      case pipeline::transitionTypeConstantView:
        to_state = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
        break;
      default:
        switch (item.stage.value) {
        case 0:
          to_state = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
          break;
        default:
          to_state = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
          break;
        }
      }

      assert(item.before == pipeline::transitionTypeInherit);

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

  ~barrier_transition () {
    if (!barriers_out.empty())
      command_list->ResourceBarrier(barriers_out.size(), barriers_out.data());
  }
};

render_pass::render_pass  (dx::render_block& block, std::array < pipeline::pass_output, 8 > const& context)
  : command_list(block.command_list)
  , pipeline(block.current_pipeline) {

  std::vector < D3D12_RENDER_PASS_RENDER_TARGET_DESC > rtv_desc;
  rtv_desc.reserve(context.size());

  std::vector < D3D12_RESOURCE_BARRIER > rtv_barrier_from;
  rtv_barrier_from.reserve(context.size());

  std::vector < D3D12_RESOURCE_BARRIER > rtv_barrier_to;
  rtv_barrier_to.reserve(context.size());

  std::vector < D3D12_VIEWPORT > rtv_viewports;
  rtv_viewports.reserve(context.size());

  std::vector < D3D12_RECT > rtv_scissors;
  rtv_scissors.reserve(context.size());

  for (const auto& item : context) {
    auto reference = item.output.cast < dx::resource_reference >();

    if (!reference) {
      continue;
    }

    D3D12_RENDER_PASS_RENDER_TARGET_DESC render_pass_desc{};

    switch (item.action_load) {
    case pipeline::loadOpLoad:
      render_pass_desc.BeginningAccess.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_PRESERVE;
      break;
    case pipeline::loadOpDoNotCare:
      render_pass_desc.BeginningAccess.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_DISCARD;
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

      render_pass_desc.BeginningAccess.Clear = clear_param;
      render_pass_desc.BeginningAccess.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR;
    } break;
    case pipeline::loadOpNoAccess:
      render_pass_desc.BeginningAccess.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_NO_ACCESS;
      break;
    }

    switch (item.action_store) {
    case pipeline::storeOpStore:
      render_pass_desc.EndingAccess.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;
      break;
    case pipeline::storeOpDoNotCare:
      render_pass_desc.EndingAccess.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_DISCARD;
      break;
    case pipeline::storeOpResolve:
      render_pass_desc.EndingAccess.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_RESOLVE;
      break;
    case pipeline::storeOpNoAccess:
      render_pass_desc.EndingAccess.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_NO_ACCESS;
      break;
    }

    render_pass_desc.cpuDescriptor = { reference->heap->GetCPUDescriptorHandleForHeapStart().ptr + reference->heap_offset };

    rtv_desc.emplace_back(render_pass_desc);

    D3D12_RESOURCE_STATES state_from;
    D3D12_RESOURCE_STATES state_during;
    D3D12_RESOURCE_STATES state_to;

    switch (item.before) {
    case pipeline::transitionTypeInherit:
      state_from = reference->created_state;
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
    default:
      state_from = D3D12_RESOURCE_STATE_RENDER_TARGET;
      break;
    }

    switch (item.during) {
    case pipeline::transitionTypeInherit:
      state_during = reference->created_state;
      break;
    default:
      state_during = D3D12_RESOURCE_STATE_RENDER_TARGET;
      break;
    }

    switch (item.after) {
    case pipeline::transitionTypeInherit:
      state_to = reference->created_state;
      break;
    case pipeline::transitionTypeShaderView:
      state_to = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
      break;
    case pipeline::transitionTypeSwapChainPresent:
      state_to = D3D12_RESOURCE_STATE_PRESENT;
      break;
    case pipeline::transitionTypeCopySource:
      state_to = D3D12_RESOURCE_STATE_COPY_SOURCE;
      break;
    case pipeline::transitionTypeCopyDestination:
      state_to = D3D12_RESOURCE_STATE_COPY_DEST;
      break;
    default:
      state_from = D3D12_RESOURCE_STATE_RENDER_TARGET;
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

    rtv_viewports.emplace_back(D3D12_VIEWPORT{
      0, 0,
      (float) item.viewport.x(), (float) item.viewport.y(),
      0.1f, 100.f });

    rtv_scissors.emplace_back(D3D12_RECT{
      (long) item.scissor.origin.x(),
      (long) item.scissor.origin.y(),

      (long) item.scissor.size.x(),
      (long)item.scissor.size.y()
    });
  }

  to_barriers = rtv_barrier_to;

  command_list->ResourceBarrier(rtv_barrier_from.size(), rtv_barrier_from.data());

  command_list->BeginRenderPass(rtv_desc.size(), rtv_desc.data(), nullptr, D3D12_RENDER_PASS_FLAG_NONE);

  command_list->RSSetViewports(rtv_viewports.size(), rtv_viewports.data());
  command_list->RSSetScissorRects(rtv_scissors.size(), rtv_scissors.data());
}

render_pass::~render_pass () {

  command_list->EndRenderPass();

  command_list->ResourceBarrier(to_barriers.size(), to_barriers.data());
}


void
  render_pass::topology       (topology_type type) {

  D3D_PRIMITIVE_TOPOLOGY dx_type = {};

  switch (type) {
  case typeTriangleList:
    dx_type = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    break;
  case typeTriganleStrip:
    dx_type = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
    break;
  case typeLineList:
    dx_type = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
    break;
  case typeLineStrip:
    dx_type = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
    break;
  case typePointList:
    dx_type = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
    break;
  }

  command_list->IASetPrimitiveTopology(dx_type);
}

cx::exp::ptr < render_pass_resource_scope >
  render_pass::buffer_prepare (std::array < pipeline::pass_input, NBS > const &transitions) {

  auto *const scope = new barrier_transition(*this, transitions);
  return cx::exp::ptr < render_pass_resource_scope, barrier_transition >{scope}.abstract();
}

void
  render_pass::buffer_bind    (pipeline::shader_stage const &binding, int slot, cx::exp::ptr_ref < gx::resource_reference > const &reference) {

  if (!reference) {
    return;
  }

  dx::resource_reference* ref = reference.cast<dx::resource_reference>().pointer();

  if (!pipeline) {
    return;
  }

  pipeline::shader_stage stage;

  switch (binding.value) {
  case 0:
    stage = pipeline::shader_stage::vertex;
    break;
  default:
    stage = pipeline::shader_stage::fragment;
    break;
  }

  auto& slot_info = pipeline->stages[stage].input;

  if (slot_info.arguments[slot].indirect) {
    command_list->SetGraphicsRootDescriptorTable(slot, {
      ref->heap->GetGPUDescriptorHandleForHeapStart().ptr + ref->heap_offset });
  }
  else {
    switch (ref->resource_type) {
    case typeSRV:
      command_list->SetGraphicsRootShaderResourceView(slot, ref->resource->GetGPUVirtualAddress());
      break;
    case typeCBV:
      command_list->SetGraphicsRootConstantBufferView(slot, ref->resource->GetGPUVirtualAddress());
      break;
    default:
      break;
    }
  }
}

void
  render_pass::buffer_bind    (pipeline::shader_stage const &binding, std::array < cx::exp::ptr_ref < gx::resource_reference >, NBS > const &references) {

  for (auto i = 0u; i < 16; i += 1) {
    if (!references[i]) {
      continue;
    }

    buffer_bind(binding, i, references[i]);
  }
}

void
  render_pass::draw           (pipeline::draw_range const &vertex_range) {
  command_list->DrawInstanced(vertex_range.size, 1, vertex_range.begin, 0);
}

SNOW_OWL_NAMESPACE_END
