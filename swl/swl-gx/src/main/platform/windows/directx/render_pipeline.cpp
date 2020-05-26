#include "directx/context.h"

#include "directx/pipeline.h"
#include "directx/render_pipeline.h"

#include <array>
#include <tell.hpp>

SNOW_OWL_NAMESPACE(gx::dx)

render_pipeline::render_pipeline  ()
  : gx::render_pipeline()
  , item_offset(0)
  , stage_offset(0) {}

render_pipeline::render_pipeline  (dx::context &context)
  : gx::render_pipeline()
  , device(context.device)
  , item_offset(0)
  , stage_offset(0) {}

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

      if (argument.type == gx::pipeline::shader_argument_type::typeNotUsed) {
        continue;
      }

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

      for (auto& argument : indirect_items) {

        D3D12_DESCRIPTOR_RANGE range{};

        range.RangeType           = gx::dx::pipeline::gx_param_table(std::get<0>(argument).type);
        range.NumDescriptors      = 1;
        
        range.RegisterSpace       = 0;
        range.BaseShaderRegister  = std::get<1>(argument);

        range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

        D3D12_ROOT_PARAMETER descriptor_param{};

        descriptor_param.ParameterType    = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        descriptor_param.ShaderVisibility = std::get<1>(visibility);

        auto item_size = sizeof D3D12_DESCRIPTOR_RANGE;

        auto *mem_range = (D3D12_DESCRIPTOR_RANGE *) calloc(1, item_size);
        memcpy_s(mem_range, item_size, &range, item_size);

        descriptor_param.DescriptorTable.NumDescriptorRanges  = 1;
        descriptor_param.DescriptorTable.pDescriptorRanges    = mem_range;

        parameters.emplace_back(descriptor_param);
      }
    }
    
    index = 0;

    for (auto& sampler : stage_item.input.samplers) {
      index += 1;
    }
  }

  D3D12_ROOT_SIGNATURE_DESC root_desc{};

  root_desc.Flags              = D3D12_ROOT_SIGNATURE_FLAG_NONE;

  root_desc.pParameters        = parameters.data();
  root_desc.NumParameters      = parameters.size();

  root_desc.pStaticSamplers    = samplers.data();
  root_desc.NumStaticSamplers  = samplers.size();

  winrt::com_ptr<ID3DBlob>    error;
  winrt::com_ptr<ID3DBlob>    result;
  D3D12SerializeRootSignature(&root_desc, D3D_ROOT_SIGNATURE_VERSION_1_0, result.put(), error.put());

  for (auto &param : parameters) {
    if (param.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE) {
      free((void *) param.DescriptorTable.pDescriptorRanges);
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

  device->CreateRootSignature(
    0,
    result->GetBufferPointer(),
    result->GetBufferSize(),
    __uuidof(ID3D12RootSignature),
    root_signature.put_void());

  D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};

  desc.RasterizerState = cx::tell < D3D12_RASTERIZER_DESC >({}, [&](D3D12_RASTERIZER_DESC &raster_desc) {

    switch (raster.cull_mode) {
    case gx::pipeline::modeBack:
      raster_desc.CullMode = D3D12_CULL_MODE_BACK;
      break;
    case gx::pipeline::modeFront:
      raster_desc.CullMode = D3D12_CULL_MODE_FRONT;
      break;
    case gx::pipeline::modeNone:
      raster_desc.CullMode = D3D12_CULL_MODE_NONE;
      break;
    }

    switch (raster.fill_mode) {
    case gx::pipeline::modeFill:
      raster_desc.FillMode = D3D12_FILL_MODE_SOLID;
      break;
    case gx::pipeline::modeWireFrame:
      raster_desc.FillMode = D3D12_FILL_MODE_WIREFRAME;
      break;
    }

    raster_desc.DepthBias            = D3D12_DEFAULT_DEPTH_BIAS;
    raster_desc.DepthBiasClamp       = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    raster_desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
  });

  desc.SampleDesc.Count   = sample.count;
  desc.SampleDesc.Quality = sample.quality;

  desc.DepthStencilState = cx::tell < D3D12_DEPTH_STENCIL_DESC >({}, [&](D3D12_DEPTH_STENCIL_DESC &stencil_desc) {
    stencil_desc.BackFace.StencilFunc         = pipeline::gx_comparison_func  (stencil.back_comparison);
    stencil_desc.BackFace.StencilPassOp       = pipeline::gx_stencil_op       (stencil.back_pass);
    stencil_desc.BackFace.StencilFailOp       = pipeline::gx_stencil_op       (stencil.back_fail);
    stencil_desc.BackFace.StencilDepthFailOp  = pipeline::gx_stencil_op       (stencil.back_depthFail);

    stencil_desc.FrontFace.StencilFunc        = pipeline::gx_comparison_func  (stencil.front_comparison);
    stencil_desc.FrontFace.StencilPassOp      = pipeline::gx_stencil_op       (stencil.front_pass);
    stencil_desc.FrontFace.StencilFailOp      = pipeline::gx_stencil_op       (stencil.front_fail);
    stencil_desc.FrontFace.StencilDepthFailOp = pipeline::gx_stencil_op       (stencil.front_depthFail);

    desc.DSVFormat                            = pipeline::gx_format(depth.format);

    stencil_desc.DepthEnable                  = depth.enabled;
    stencil_desc.DepthFunc                    = pipeline::gx_comparison_func(depth.comparison);
    stencil_desc.DepthWriteMask               = D3D12_DEPTH_WRITE_MASK_ALL;

    stencil_desc.StencilEnable                = stencil.enabled;
    stencil_desc.StencilReadMask              = D3D12_DEFAULT_STENCIL_READ_MASK;
    stencil_desc.StencilWriteMask             = D3D12_DEFAULT_STENCIL_WRITE_MASK;
  });

  size_t num_targets = 0;
  size_t index = 0;

  for (auto &output : stages[gx::pipeline::shader_stage::fragment].output) {

    index += 1;

    if (output.format == gx::pipeline::format_unknown) {
      continue;
    }

    num_targets += 1;

    auto rtv_index = index - 1;

    desc.RTVFormats[rtv_index] = pipeline::gx_format(output.format);

    desc.BlendState.RenderTarget[rtv_index].BlendEnable           = output.blend.enabled;
    desc.BlendState.RenderTarget[rtv_index].LogicOpEnable         = output.blend.op_enabled;

    desc.BlendState.RenderTarget[rtv_index].LogicOp               = D3D12_LOGIC_OP_NOOP;

    desc.BlendState.RenderTarget[rtv_index].BlendOp               = pipeline::gx_blend_op(output.blend.blend_operation);
    desc.BlendState.RenderTarget[rtv_index].BlendOpAlpha          = pipeline::gx_blend_op(output.blend.blend_alpha_operation);

    desc.BlendState.RenderTarget[rtv_index].SrcBlend              = pipeline::gx_blend(output.blend.source_blend);
    desc.BlendState.RenderTarget[rtv_index].SrcBlendAlpha         = pipeline::gx_blend(output.blend.source_alpha_blend);
    
    desc.BlendState.RenderTarget[rtv_index].DestBlend             = pipeline::gx_blend(output.blend.destination_blend);
    desc.BlendState.RenderTarget[rtv_index].DestBlendAlpha        = pipeline::gx_blend(output.blend.destination_alpha_blend);

    desc.BlendState.RenderTarget[rtv_index].RenderTargetWriteMask = output.blend.write_mask;
  }

  desc.NumRenderTargets                   = num_targets;
  desc.BlendState.IndependentBlendEnable  = true;

  desc.pRootSignature         = root_signature.get();
  desc.SampleMask             = UINT_MAX;
  desc.PrimitiveTopologyType  = pipeline::gx_primitive(topology_type);

  desc.DS = D3D12_SHADER_BYTECODE{};
  desc.HS = D3D12_SHADER_BYTECODE{};
  desc.GS = D3D12_SHADER_BYTECODE{};

  auto &vs_func = stages[gx::pipeline::shader_stage::vertex].function;
  desc.VS = {vs_func.byte_code, vs_func.byte_size};

  auto &ps_func = stages[gx::pipeline::shader_stage::fragment].function;
  desc.PS = {ps_func.byte_code, ps_func.byte_size};

  winrt::check_hresult(device->CreateGraphicsPipelineState(&desc,
    __uuidof(ID3D12PipelineState), pipeline_state.put_void()));

  D3D12_DESCRIPTOR_HEAP_DESC heap_desc{};

  heap_desc.NumDescriptors  = NBS * stages.size();
  heap_desc.Flags           = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
  heap_desc.Type            = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

  winrt::check_hresult(device->CreateDescriptorHeap(&heap_desc,
    __uuidof(ID3D12DescriptorHeap), descriptor_buf.put_void()));

  item_offset   = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
  stage_offset  = NBS * item_offset;
}

SNOW_OWL_NAMESPACE_END
