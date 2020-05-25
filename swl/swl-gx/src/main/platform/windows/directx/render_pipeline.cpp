#include "directx/context.h"

#include "directx/pipeline.h"
#include "directx/render_pipeline.h"

#include <array>
#include <tell.hpp>

SNOW_OWL_NAMESPACE(gx::dx)

render_pipeline::render_pipeline  ()
  : gx::render_pipeline() {}

render_pipeline::render_pipeline  (dx::context &context)
  : gx::render_pipeline()
  , device(context.device) {}

void
  render_pipeline::construct      () {

  std::array pipeline_resource_mapping{
      std::make_pair(gx::pipeline::shader_stage::vertex,    D3D12_SHADER_VISIBILITY_VERTEX  ),
      std::make_pair(gx::pipeline::shader_stage::fragment,  D3D12_SHADER_VISIBILITY_PIXEL   ),
  };

  std::vector < D3D12_ROOT_PARAMETER >      parameters  {};
  std::vector < D3D12_STATIC_SAMPLER_DESC > samplers    {};

  for (auto visibility : pipeline_resource_mapping) {

    uint16_t index = 0;

    std::vector<std::pair<gx::pipeline::shader_argument, uint16_t>> indirect_items;
    indirect_items.reserve(NBS);

    gx::pipeline::pipeline_stage &stage_item = this->stages[get<0>(visibility)];

    for (auto &argument : stage_item.input.arguments) {

      if (argument.indirect) {
        indirect_items.emplace_back(argument, index);
      }
      else {
        D3D12_ROOT_PARAMETER param{};

        param.ParameterType             = gx::dx::pipeline::gx_param(argument.type);

        param.Descriptor.RegisterSpace  = 0;
        param.Descriptor.ShaderRegister = index;

        param.ShaderVisibility          = std::get<1>(visibility);

        parameters.emplace_back(param);
      }

      index += 1;
    }

    if (!indirect_items.empty()) {

      std::vector<D3D12_DESCRIPTOR_RANGE> ranges;
      ranges.reserve(indirect_items.size());

      for (auto& argument : indirect_items) {

        D3D12_DESCRIPTOR_RANGE range{};

        range.RangeType           = gx::dx::pipeline::gx_param_table(std::get<0>(argument).type);
        range.NumDescriptors      = 1;
        
        range.RegisterSpace       = 0;
        range.BaseShaderRegister  = std::get<1>(argument);

        range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

        ranges.emplace_back(std::move(range));
      }

      D3D12_ROOT_PARAMETER descriptor_param{};

      descriptor_param.ParameterType    = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
      descriptor_param.ShaderVisibility = std::get<1>(visibility);

      descriptor_param.DescriptorTable.NumDescriptorRanges  = ranges.size();
      descriptor_param.DescriptorTable.pDescriptorRanges    = ranges.data();

      parameters.emplace_back(descriptor_param);
    }
    
    D3D12_DESCRIPTOR_HEAP_DESC heap_desc{};

    heap_desc.NumDescriptors  = NBS * stages.size();
    heap_desc.Flags           = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    heap_desc.Type            = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

    winrt::check_hresult(device->CreateDescriptorHeap(&heap_desc,
      __uuidof(ID3D12DescriptorHeap), descriptor_buf.put_void()));

    item_offset   = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    stage_offset  = NBS * item_offset;

    index = 0;

    for (auto& sampler : stage_item.input.samplers) {

      index += 1;
    }
  }

  D3D12_ROOT_SIGNATURE_DESC rootDesc{};

  rootDesc.Flags              = D3D12_ROOT_SIGNATURE_FLAG_NONE;

  rootDesc.pParameters        = parameters.data();
  rootDesc.NumParameters      = parameters.size();

  rootDesc.pStaticSamplers    = samplers.data();
  rootDesc.NumStaticSamplers  = samplers.size();

  winrt::com_ptr<ID3DBlob>    error;
  winrt::com_ptr<ID3DBlob>    result;
  D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, result.put(), error.put());

  for (auto& item : parameters) {
    if (item.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE) {
      delete item.DescriptorTable.pDescriptorRanges;
    }
  }

  if (error) {
    std::string error_blob;
    error_blob.reserve(error->GetBufferSize());

    memcpy(error_blob.data(), error->GetBufferPointer(), error->GetBufferSize());

    size_t w_size = MultiByteToWideChar(CP_UTF8, 0, error_blob.c_str(), -1, nullptr, 0);

    std::wstring message;
    message.reserve(w_size);

    MultiByteToWideChar(CP_UTF8, 0, error_blob.c_str(), -1, message.data(), w_size);

    OutputDebugString(message.c_str());
  }

  device->CreateRootSignature(0, result.get()->GetBufferPointer(), result->GetBufferSize(), __uuidof(ID3D12RootSignature), root_signature.put_void());

  D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};

  desc.RasterizerState = cx::tell < D3D12_RASTERIZER_DESC >({}, [&](D3D12_RASTERIZER_DESC &desc) {

    switch (raster.cull_mode) {
    case gx::pipeline::modeBack:
      desc.CullMode = D3D12_CULL_MODE_BACK;
      break;
    case gx::pipeline::modeFront:
      desc.CullMode = D3D12_CULL_MODE_FRONT;
      break;
    case gx::pipeline::modeNone:
      desc.CullMode = D3D12_CULL_MODE_NONE;
      break;
    }

    switch (raster.fill_mode) {
    case gx::pipeline::modeFill:
      desc.FillMode = D3D12_FILL_MODE_SOLID;
      break;
    case gx::pipeline::modeWireFrame:
      desc.FillMode = D3D12_FILL_MODE_WIREFRAME;
      break;
    }

    desc.DepthBias            = D3D12_DEFAULT_DEPTH_BIAS;
    desc.DepthBiasClamp       = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
  });

  desc.SampleDesc.Count   = sample.count;
  desc.SampleDesc.Quality = sample.quality;

  desc.DepthStencilState = cx::tell < D3D12_DEPTH_STENCIL_DESC >({}, [&](D3D12_DEPTH_STENCIL_DESC &desc) {
    desc.BackFace.StencilFunc         = gx::dx::pipeline::gx_comparison_func  (stencil.back_comparison);
    desc.BackFace.StencilPassOp       = gx::dx::pipeline::gx_stencil_op       (stencil.back_pass);
    desc.BackFace.StencilFailOp       = gx::dx::pipeline::gx_stencil_op       (stencil.back_fail);
    desc.BackFace.StencilDepthFailOp  = gx::dx::pipeline::gx_stencil_op       (stencil.back_depthFail);

    desc.FrontFace.StencilFunc        = gx::dx::pipeline::gx_comparison_func  (stencil.front_comparison);
    desc.FrontFace.StencilPassOp      = gx::dx::pipeline::gx_stencil_op       (stencil.front_pass);
    desc.FrontFace.StencilFailOp      = gx::dx::pipeline::gx_stencil_op       (stencil.front_fail);
    desc.FrontFace.StencilDepthFailOp = gx::dx::pipeline::gx_stencil_op       (stencil.front_depthFail);

    desc.DepthEnable                  = depth.enabled;
    desc.DepthFunc                    = gx::dx::pipeline::gx_comparison_func(depth.comparison);
    desc.DepthWriteMask               = D3D12_DEPTH_WRITE_MASK_ALL;

    desc.StencilEnable                = stencil.enabled;
    desc.StencilReadMask              = D3D12_DEFAULT_STENCIL_READ_MASK;
    desc.StencilWriteMask             = D3D12_DEFAULT_STENCIL_WRITE_MASK;
  });

  desc.pRootSignature         = root_signature.get();
  desc.PrimitiveTopologyType  = gx::dx::pipeline::gx_primitive(topology_type);

  desc.DS = D3D12_SHADER_BYTECODE{};
  desc.HS = D3D12_SHADER_BYTECODE{};
  desc.GS = D3D12_SHADER_BYTECODE{};

  desc.SampleMask           = UINT_MAX;
  desc.NumRenderTargets     = 0;

  winrt::check_hresult(device->CreateGraphicsPipelineState(&desc,
    __uuidof(ID3D12PipelineState), pipeline_state.put_void()));
}

SNOW_OWL_NAMESPACE_END
