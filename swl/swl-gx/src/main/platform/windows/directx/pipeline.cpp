#include "directx/pipeline.h"

#include <array>
#include <tell.hpp>

SNOW_OWL_NAMESPACE(gx::dx)

constexpr DXGI_FORMAT
  gx_format(const gx::pipeline::format &format) {

  if (format == 0) { return DXGI_FORMAT_UNKNOWN; }

  const std::array _uxxx{
      DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN,
      DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN,
      DXGI_FORMAT_UNKNOWN,
  };

  const std::array _32_4{
      DXGI_FORMAT_R32G32B32A32_SINT, DXGI_FORMAT_UNKNOWN,
      DXGI_FORMAT_R32G32B32A32_UINT, DXGI_FORMAT_UNKNOWN,
      DXGI_FORMAT_R32G32B32A32_FLOAT,
  };

  const std::array _32_3{
      DXGI_FORMAT_R32G32B32_SINT, DXGI_FORMAT_UNKNOWN,
      DXGI_FORMAT_R32G32B32_UINT, DXGI_FORMAT_UNKNOWN,
      DXGI_FORMAT_R32G32B32_FLOAT,
  };

  const std::array _32_2{
      DXGI_FORMAT_R32G32_SINT, DXGI_FORMAT_UNKNOWN,
      DXGI_FORMAT_R32G32_UINT, DXGI_FORMAT_UNKNOWN,
      DXGI_FORMAT_R32G32_FLOAT,
  };

  const std::array _32_1{
      DXGI_FORMAT_R32_SINT, DXGI_FORMAT_UNKNOWN,
      DXGI_FORMAT_R32_UINT, DXGI_FORMAT_UNKNOWN,
      DXGI_FORMAT_R32_FLOAT,
  };

  const std::array _16_4{
      DXGI_FORMAT_R16G16B16A16_SINT, DXGI_FORMAT_R16G16B16A16_SNORM,
      DXGI_FORMAT_R16G16B16A16_UINT, DXGI_FORMAT_R16G16B16A16_UNORM,
      DXGI_FORMAT_R16G16B16A16_FLOAT,
  };

  const std::array _16_2{
      DXGI_FORMAT_R16G16_SINT, DXGI_FORMAT_R16G16_SNORM,
      DXGI_FORMAT_R16G16_UINT, DXGI_FORMAT_R16G16_UNORM,
      DXGI_FORMAT_R16G16_FLOAT,
  };

  const std::array _16_1{
      DXGI_FORMAT_R16_SINT, DXGI_FORMAT_R16_SNORM,
      DXGI_FORMAT_R16_UINT, DXGI_FORMAT_R16_UNORM,
      DXGI_FORMAT_R16_FLOAT,
  };

  const std::array _8_4{
      DXGI_FORMAT_R8G8B8A8_SINT, DXGI_FORMAT_R8G8B8A8_SNORM,
      DXGI_FORMAT_R8G8B8A8_UINT, DXGI_FORMAT_R8G8B8A8_UNORM,
      DXGI_FORMAT_UNKNOWN
  };

  const std::array _8_2{
      DXGI_FORMAT_R8G8_SINT, DXGI_FORMAT_R8G8_SNORM,
      DXGI_FORMAT_R8G8_UINT, DXGI_FORMAT_R8G8_UNORM,
      DXGI_FORMAT_UNKNOWN
  };

  const std::array _8_1{
      DXGI_FORMAT_R8G8_SINT, DXGI_FORMAT_R8G8_SNORM,
      DXGI_FORMAT_R8G8_UINT, DXGI_FORMAT_R8G8_UNORM,
      DXGI_FORMAT_UNKNOWN
  };

  const std::array _depth{
      DXGI_FORMAT_D32_FLOAT,
      DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
      DXGI_FORMAT_D24_UNORM_S8_UINT,
      DXGI_FORMAT_D16_UNORM,
  };

  const std::array _swapc{
      DXGI_FORMAT_B8G8R8A8_TYPELESS,
      DXGI_FORMAT_B8G8R8A8_UNORM,
  };

  const std::array component_4{_8_4, _16_4, _uxxx, _32_4};
  const std::array component_3{_uxxx, _uxxx, _uxxx, _32_3};
  const std::array component_2{_8_2, _16_2, _uxxx, _32_2};
  const std::array component_1{_8_1, _16_1, _uxxx, _32_1};

  const std::array components{component_1, component_2, component_3, component_4};

  if ((0x0000'B100 & format) == 0xB100) {
    const int index = ((0x00FF'0000 & format) >> 16) - 1;
    return _swapc[index];
  }

  if ((0x0000'D400 & format) == 0xD400) {
    const int index = ((0x00FF'0000 & format) >> 16) - 1;
    return _depth[index];
  }

  const int component = (0x00FF'0000 & format >> 16) - 1;
  const int byte      = (0x0000'FF00 & format >> 8) - 1;
  const int type      = (0x0000'00FF & format);

  return components[component][byte][type];
}

constexpr D3D12_ROOT_PARAMETER_TYPE
  gx_param(const gx::pipeline::render_input_type &type) {
  switch (type) {
  case pipeline::typeConstant:
    return D3D12_ROOT_PARAMETER_TYPE_CBV;
  case pipeline::typeBuffer:
  case pipeline::typeTexture:
  case pipeline::typeBufferUser:
    return D3D12_ROOT_PARAMETER_TYPE_SRV;
  }

  return {};
}

constexpr D3D12_DESCRIPTOR_RANGE_TYPE
  gx_param_table(const gx::pipeline::render_input_type &type) {
  switch (type) {
  case pipeline::typeConstant:
    return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
  case pipeline::typeBuffer:
  case pipeline::typeTexture:
  case pipeline::typeBufferUser:
    return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
  }

  return {};
}

constexpr D3D12_PRIMITIVE_TOPOLOGY_TYPE
  gx_primitive(const gx::pipeline::topology_type &type) {
  switch (type) {
  case pipeline::topologyTypePoint:
    return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
  case pipeline::topologyTypeLine:
    return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
  case pipeline::topologyTypeTriangle:
    return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
  }

  return {};
}

constexpr D3D12_BLEND
  gx_blend(const gx::pipeline::blend_type &type) {

  if (type == pipeline::typeZero) { return D3D12_BLEND_ZERO; }

  if (type == pipeline::typeOne) { return D3D12_BLEND_ONE; }

  const std::array _s_c{D3D12_BLEND_SRC_COLOR, D3D12_BLEND_INV_SRC_COLOR};
  const std::array _s_a{D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_INV_SRC_ALPHA};

  const std::array _d_c{D3D12_BLEND_DEST_COLOR, D3D12_BLEND_INV_DEST_COLOR};
  const std::array _d_a{D3D12_BLEND_DEST_ALPHA, D3D12_BLEND_INV_DEST_ALPHA};

  const std::array _b_x{D3D12_BLEND_BLEND_FACTOR, D3D12_BLEND_INV_BLEND_FACTOR};

  const std::array _s_l{_s_c, _s_a};
  const std::array _d_l{_d_c, _d_a};
  const std::array _b_l{_b_x, std::array{D3D12_BLEND_ZERO, D3D12_BLEND_ZERO}};

  std::array list{_s_l, _d_l, _b_l};

  const int location  = ((0xFFFF'0000 & type) >> 16) - 1;
  const int component = ((0x0000'FF00 & type) >> 8) - 1;
  const int kind      = (0x0000'00FF & type) - 1;

  return list[location][component][kind];
}

constexpr D3D12_BLEND_OP
  gx_blend_op(const gx::pipeline::blend_op &op) {

  switch (op) {
  case pipeline::opAdd:
    return D3D12_BLEND_OP_ADD;
  case pipeline::opMax:
    return D3D12_BLEND_OP_MAX;
  case pipeline::opMin:
    return D3D12_BLEND_OP_MIN;
  case pipeline::opSubtract:
    return D3D12_BLEND_OP_SUBTRACT;
  }

  return D3D12_BLEND_OP{};
}

constexpr D3D12_TEXTURE_ADDRESS_MODE
gx_address_mode(const gx::pipeline::address_mode& mode) {
  switch (mode) {
  case pipeline::modeClamp:
    return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
  case pipeline::modeWrap:
    return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
  case pipeline::modeMirror:
    return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
  }
}

constexpr D3D12_COMPARISON_FUNC
gx_comparison_func(gx::pipeline::comparison_type const& type) {
  switch (type) {
  case pipeline::comparisonAlways:
    return D3D12_COMPARISON_FUNC_ALWAYS;
  case pipeline::comparisonNever:
    return D3D12_COMPARISON_FUNC_NEVER;
  case pipeline::comparisonEqual:
    return D3D12_COMPARISON_FUNC_EQUAL;
  case pipeline::comparisonNotEqual:
    return D3D12_COMPARISON_FUNC_NOT_EQUAL;
  case pipeline::comparisonLess:
    return D3D12_COMPARISON_FUNC_LESS;
  case pipeline::comparisonLessEqual:
    return D3D12_COMPARISON_FUNC_LESS_EQUAL;
  case pipeline::comparisonMore:
    return D3D12_COMPARISON_FUNC_GREATER;
  case pipeline::comparisonMoreEqual:
    return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
  }
}

constexpr D3D12_STENCIL_OP
gx_stencil_op(gx::pipeline::stencil_op const& type) {
  switch (type) {
  case pipeline::opKeep:
    return D3D12_STENCIL_OP_KEEP;
  case pipeline::opReplace:
    return D3D12_STENCIL_OP_REPLACE;
  case pipeline::opZero:
    return D3D12_STENCIL_OP_ZERO;
  case pipeline::opSatIncrease:
    return D3D12_STENCIL_OP_INCR_SAT;
  case pipeline::opSatDecrease:
    return D3D12_STENCIL_OP_DECR_SAT;
  case pipeline::opInvert:
    return D3D12_STENCIL_OP_INVERT;
  case pipeline::opIncrease:
    return D3D12_STENCIL_OP_INCR;
  case pipeline::opDecrease:
    return D3D12_STENCIL_OP_DECR;
  }
}


render_pipeline::render_pipeline()
  : gx::render_pipeline() {}

render_pipeline::render_pipeline(dx::context &context)
  : gx::render_pipeline()
  , device(context.device) {}

void
  render_pipeline::construct() {

  std::array pipeline_resource_mapping{
      std::make_pair(pipeline::shader_stage::vertex, D3D12_SHADER_VISIBILITY_VERTEX),
      std::make_pair(pipeline::shader_stage::fragment, D3D12_SHADER_VISIBILITY_PIXEL),
  };

  std::vector < D3D12_ROOT_PARAMETER > parameters{};
  std::vector < D3D12_STATIC_SAMPLER_DESC > samplers{};

  for (auto visibility : pipeline_resource_mapping) {

    uint16_t index = 0;

    for (const auto binding : render_inputs[visibility.first].bindings) {
      if (binding.type == gx::pipeline::typeNotUsed) {
        index += 1;
        continue;
      }

      D3D12_ROOT_PARAMETER param;
      ZeroMemory(&param, sizeof D3D12_ROOT_PARAMETER);

      D3D12_DESCRIPTOR_RANGE* table_range = new D3D12_DESCRIPTOR_RANGE();
      ZeroMemory(table_range, sizeof D3D12_DESCRIPTOR_RANGE);

      if (binding.indirect) {

        table_range->RangeType = gx_param_table(binding.type);

        table_range->BaseShaderRegister = index;
        table_range->NumDescriptors = 1;
        table_range->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

        param.ParameterType   = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        
        param.DescriptorTable.NumDescriptorRanges = 1;
        param.DescriptorTable.pDescriptorRanges   = table_range;
      } else {
        param.ParameterType = gx_param(binding.type);
        param.Descriptor    = D3D12_ROOT_DESCRIPTOR{static_cast < UINT >(index), 0};
      }

      param.ShaderVisibility = visibility.second;

      parameters.emplace_back(std::move(param));

      index += 1;
    }
  }

  D3D12_ROOT_SIGNATURE_DESC rootDesc{};

  rootDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;

  rootDesc.pParameters   = parameters.data();
  rootDesc.NumParameters = parameters.size();

  /*
  rootDesc.pStaticSamplers   = samplers.data();
  rootDesc.NumStaticSamplers = samplers.size();
  */
  winrt::com_ptr < ID3DBlob > result;
  winrt::com_ptr < ID3DBlob > error;
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
    case pipeline::modeBack:
      desc.CullMode = D3D12_CULL_MODE_BACK;
      break;
    case pipeline::modeFront:
      desc.CullMode = D3D12_CULL_MODE_FRONT;
      break;
    case pipeline::modeNone:
      desc.CullMode = D3D12_CULL_MODE_NONE;
      break;
    }

    switch (raster.fill_mode) {
    case pipeline::modeFill:
      desc.FillMode = D3D12_FILL_MODE_SOLID;
      break;
    case pipeline::modeWireFrame:
      desc.FillMode = D3D12_FILL_MODE_WIREFRAME;
      break;
    }

    desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
  });

  desc.SampleDesc.Count = 1;
  desc.SampleDesc.Quality = 0;

  desc.DSVFormat = gx_format(pipeline::format_1_32_float_depth);

  desc.DepthStencilState = cx::tell < D3D12_DEPTH_STENCIL_DESC >({}, [&](D3D12_DEPTH_STENCIL_DESC &desc) {
    desc.BackFace.StencilFunc = gx_comparison_func(stencil.back_comparison);
    desc.BackFace.StencilPassOp = gx_stencil_op(stencil.back_pass);
    desc.BackFace.StencilFailOp = gx_stencil_op(stencil.back_fail);
    desc.BackFace.StencilDepthFailOp = gx_stencil_op(stencil.back_depthFail);

    desc.FrontFace.StencilFunc = gx_comparison_func(stencil.front_comparison);
    desc.FrontFace.StencilPassOp = gx_stencil_op(stencil.front_pass);
    desc.FrontFace.StencilFailOp = gx_stencil_op(stencil.front_fail);
    desc.FrontFace.StencilDepthFailOp = gx_stencil_op(stencil.front_depthFail);

    desc.DepthEnable = depth.enabled;
    desc.DepthFunc = gx_comparison_func(depth.comparison);
    desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;

    desc.StencilEnable = stencil.enabled;
    desc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
    desc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
  });

  desc.pRootSignature = root_signature.get();
  desc.PrimitiveTopologyType = gx_primitive(topology_type);

  desc.DS = D3D12_SHADER_BYTECODE{};
  desc.HS = D3D12_SHADER_BYTECODE{};
  desc.GS = D3D12_SHADER_BYTECODE{};

  auto& vs_stage = shader_stages[pipeline::shader_stage::vertex];
  auto& ps_stage = shader_stages[pipeline::shader_stage::fragment];

  desc.VS = D3D12_SHADER_BYTECODE{ vs_stage.byte_code, vs_stage.byte_size };
  desc.PS = D3D12_SHADER_BYTECODE{ ps_stage.byte_code, ps_stage.byte_size };

  auto numRTVs = 0u;
  for (auto i = 0u; i < render_outputs.size(); i += 1) {
    pipeline::render_output &output = render_outputs[i];

    if (output.format == pipeline::format_unknown) {
      continue;
    }

    numRTVs += 1;
    desc.RTVFormats[i] = gx_format(output.format);

    desc.BlendState.IndependentBlendEnable = true;
    desc.BlendState.RenderTarget[i] = cx::tell < D3D12_RENDER_TARGET_BLEND_DESC >({}, [&output](D3D12_RENDER_TARGET_BLEND_DESC &blend) {
      blend.BlendEnable   = output.blend.enabled;
      blend.LogicOpEnable = output.blend.op_enabled;

      blend.SrcBlend      = gx_blend(output.blend.source_blend);
      blend.SrcBlendAlpha = gx_blend(output.blend.source_alpha_blend);

      blend.DestBlend      = gx_blend(output.blend.destination_blend);
      blend.DestBlendAlpha = gx_blend(output.blend.destination_alpha_blend);

      blend.BlendOp      = gx_blend_op(output.blend.blend_operation);
      blend.BlendOpAlpha = gx_blend_op(output.blend.blend_alpha_operation);

      blend.LogicOp               = D3D12_LOGIC_OP_NOOP;
      blend.RenderTargetWriteMask = output.blend.write_mask;
    });
  }

  desc.SampleMask = UINT_MAX;
  desc.NumRenderTargets = numRTVs;
  
  device->CreateGraphicsPipelineState(&desc, __uuidof(ID3D12PipelineState), pipeline_state.put_void());
}

SNOW_OWL_NAMESPACE_END
