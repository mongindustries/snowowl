#include "directx/pipeline.h"

#include <array>
#include <tell.hpp>

SNOW_OWL_NAMESPACE(gx::dx)

render_pipeline::render_pipeline(): gx::render_pipeline() {

}

render_pipeline::render_pipeline(dx::context& context) : gx::render_pipeline() {

  std::array parameters{
    cx::tell<D3D12_ROOT_PARAMETER>({}, [](D3D12_ROOT_PARAMETER& param) {
      param.ParameterType     = D3D12_ROOT_PARAMETER_TYPE_CBV;
      param.Descriptor        = D3D12_ROOT_DESCRIPTOR{ 0, 0 };
      param.ShaderVisibility  = D3D12_SHADER_VISIBILITY_VERTEX;
    })
  };

  D3D12_ROOT_SIGNATURE_DESC rootDesc{};

  rootDesc.Flags          = D3D12_ROOT_SIGNATURE_FLAG_NONE;

  rootDesc.pParameters    = parameters.data();
  rootDesc.NumParameters  = parameters.size();

  winrt::com_ptr<ID3DBlob> result;
  D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, result.put(), nullptr);

  context.device->CreateRootSignature(0, result.get(), result->GetBufferSize(), __uuidof(ID3D12RootSignature), nullptr);

  D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};

  desc.NumRenderTargets = render_outputs.size();

  context.device->CreateGraphicsPipelineState(&desc, __uuidof(ID3D12PipelineState), pipeline_state.put_void());
}

SNOW_OWL_NAMESPACE_END
