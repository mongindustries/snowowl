#include "directx/pipeline.h"

#include <array>
#include <tell.hpp>

SNOW_OWL_NAMESPACE(gx::dx)

constexpr DXGI_FORMAT
  gx_format     (const gx::pipeline::format &format) {

  if (format == 0) {
    return DXGI_FORMAT_UNKNOWN;
  }

  const std::array _uxxx{
    DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN, };

  const std::array _32_4{
    DXGI_FORMAT_R32G32B32A32_SINT, DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_R32G32B32A32_UINT, DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_R32G32B32A32_FLOAT, };

  const std::array _32_3{
    DXGI_FORMAT_R32G32B32_SINT, DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_R32G32B32_UINT, DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_R32G32B32_FLOAT, };

  const std::array _32_2{
    DXGI_FORMAT_R32G32_SINT, DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_R32G32_UINT, DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_R32G32_FLOAT, };

  const std::array _32_1{
    DXGI_FORMAT_R32_SINT, DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_R32_UINT, DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_R32_FLOAT, };

  const std::array _16_4{
    DXGI_FORMAT_R16G16B16A16_SINT, DXGI_FORMAT_R16G16B16A16_SNORM,
    DXGI_FORMAT_R16G16B16A16_UINT, DXGI_FORMAT_R16G16B16A16_UNORM,
    DXGI_FORMAT_R16G16B16A16_FLOAT, };

  const std::array _16_2{
    DXGI_FORMAT_R16G16_SINT, DXGI_FORMAT_R16G16_SNORM,
    DXGI_FORMAT_R16G16_UINT, DXGI_FORMAT_R16G16_UNORM,
    DXGI_FORMAT_R16G16_FLOAT, };

  const std::array _16_1{
    DXGI_FORMAT_R16_SINT, DXGI_FORMAT_R16_SNORM,
    DXGI_FORMAT_R16_UINT, DXGI_FORMAT_R16_UNORM,
    DXGI_FORMAT_R16_FLOAT, };

  const std::array _8_4{
    DXGI_FORMAT_R8G8B8A8_SINT, DXGI_FORMAT_R8G8B8A8_SNORM,
    DXGI_FORMAT_R8G8B8A8_UINT, DXGI_FORMAT_R8G8B8A8_UNORM,
    DXGI_FORMAT_UNKNOWN };

  const std::array _8_2{
    DXGI_FORMAT_R8G8_SINT, DXGI_FORMAT_R8G8_SNORM,
    DXGI_FORMAT_R8G8_UINT, DXGI_FORMAT_R8G8_UNORM,
    DXGI_FORMAT_UNKNOWN };

  const std::array _8_1{
    DXGI_FORMAT_R8G8_SINT, DXGI_FORMAT_R8G8_SNORM,
    DXGI_FORMAT_R8G8_UINT, DXGI_FORMAT_R8G8_UNORM,
    DXGI_FORMAT_UNKNOWN };

  const std::array _depth{
    DXGI_FORMAT_D32_FLOAT,
    DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
    DXGI_FORMAT_D24_UNORM_S8_UINT,
    DXGI_FORMAT_D16_UNORM, };

  const std::array _swapc{
    DXGI_FORMAT_B8G8R8A8_TYPELESS,
    DXGI_FORMAT_B8G8R8A8_UNORM, };

  const std::array component_4  {  _8_4, _16_4, _uxxx, _32_4 };
  const std::array component_3  { _uxxx, _uxxx, _uxxx, _32_3 };
  const std::array component_2  {  _8_2, _16_2, _uxxx, _32_2 };
  const std::array component_1  {  _8_1, _16_1, _uxxx, _32_1 };

  const std::array components   { component_1, component_2, component_3, component_4 };

  if (0x0000'D000 & format) {
    const int index = (0x00FF'0000 & format) >> 16;
    return _depth[index];
  }

  if (0x0000'B000 & format) {
    const int index = (0x00FF'0000 & format) >> 16;
    return _swapc[index];
  }

  const int component  = (0x00FF'0000 & format >> 16) - 1;
  const int byte       = (0x0000'FF00 & format >>  8) - 1;
  const int type       = (0x0000'00FF & format);

  return components[component][byte][type];
}

constexpr D3D12_ROOT_PARAMETER_TYPE
  gx_param      (const gx::pipeline::render_input_type &type) {
  switch (type) {
  case pipeline::typeConstant:
    return D3D12_ROOT_PARAMETER_TYPE_CBV;
  case pipeline::typeBuffer:
  case pipeline::typeTexture:
    return D3D12_ROOT_PARAMETER_TYPE_SRV;
  }

  return {};
}

constexpr D3D12_PRIMITIVE_TOPOLOGY_TYPE
  gx_primitive  (const gx::pipeline::topology_type &type) {
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
  gx_blend      (const gx::pipeline::blend_type& type) {

  const std::array _s_c{ D3D12_BLEND_SRC_COLOR, D3D12_BLEND_INV_SRC_COLOR };
  const std::array _s_a{ D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_INV_SRC_ALPHA };

  const std::array _d_c{ D3D12_BLEND_DEST_COLOR, D3D12_BLEND_INV_DEST_COLOR };
  const std::array _d_a{ D3D12_BLEND_DEST_ALPHA, D3D12_BLEND_INV_DEST_ALPHA };

  const std::array _b_x{ D3D12_BLEND_BLEND_FACTOR, D3D12_BLEND_INV_BLEND_FACTOR };

  const std::array _s_l{ _s_c, _s_a };
  const std::array _d_l{ _d_c, _d_a };
  const std::array _b_l{ _b_x, std::array{ D3D12_BLEND_ZERO, D3D12_BLEND_ZERO } };

  std::array list{ _s_l, _d_l, _b_l };

  const int location   = ((0xFFFF'0000 & type) >> 16) - 1;
  const int component  = ((0x0000'FF00 & type) >>  8) - 1;
  const int kind       =  (0x0000'00FF & type)        - 1;

  return list[location][component][kind];
}

constexpr D3D12_BLEND_OP
  gx_blend_op   (const gx::pipeline::blend_op& op) {
  
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


render_pipeline::render_pipeline(): gx::render_pipeline() {

}

render_pipeline::render_pipeline(dx::context& context) : gx::render_pipeline() {

  std::array pipeline_resource_mapping{
    std::make_pair(pipeline::shader_stage::vertex,    D3D12_SHADER_VISIBILITY_VERTEX),
    std::make_pair(pipeline::shader_stage::fragment,  D3D12_SHADER_VISIBILITY_PIXEL ),
  };

  std::vector<D3D12_ROOT_PARAMETER> parameters{};
  for (auto& visibility : pipeline_resource_mapping) {
    for (const auto& binding : render_inputs[visibility.first].resource_binding) {
      parameters.emplace_back(cx::tell<D3D12_ROOT_PARAMETER>({}, [&binding, &visibility](D3D12_ROOT_PARAMETER& param) {
        param.ParameterType     = gx_param(binding.type);
        param.Descriptor        = D3D12_ROOT_DESCRIPTOR{ static_cast<UINT>(binding.location), static_cast<UINT>(binding.region) };
        param.ShaderVisibility  = visibility.second;
      }));
    }
  }

  std::vector<D3D12_SAMPLER_DESC> samplers{};

  samplers.emplace_back(cx::tell<D3D12_SAMPLER_DESC>({}, [](D3D12_SAMPLER_DESC& desc) {

    desc.AddressU;
    desc.AddressV;
    desc.AddressW;

    desc.Filter;
    desc.MaxAnisotropy;

    desc.ComparisonFunc;

    desc.MinLOD;
    desc.MaxLOD;
    desc.MipLODBias;
  }));

  D3D12_ROOT_SIGNATURE_DESC rootDesc{};

  rootDesc.Flags          = D3D12_ROOT_SIGNATURE_FLAG_NONE;

  rootDesc.pParameters    = parameters.data();
  rootDesc.NumParameters  = parameters.size();

  winrt::com_ptr<ID3DBlob> result;
  D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, result.put(), nullptr);

  context.device->CreateRootSignature(0, result.get(), result->GetBufferSize(), __uuidof(ID3D12RootSignature), root_signature.put_void());

  D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};

  desc.RasterizerState        = cx::tell<D3D12_RASTERIZER_DESC>({}, [](D3D12_RASTERIZER_DESC& desc) {

    (void) desc.CullMode;
    (void) desc.FillMode;
    (void) desc.FrontCounterClockwise;

    (void) desc.DepthBias;
    (void) desc.DepthBiasClamp;
    (void) desc.DepthClipEnable;
  });
  desc.DepthStencilState      = cx::tell<D3D12_DEPTH_STENCIL_DESC>({}, [](D3D12_DEPTH_STENCIL_DESC& desc) {
  
    (void) desc.BackFace;
    (void) desc.FrontFace;

    (void) desc.DepthEnable;
    (void) desc.DepthFunc;
    (void) desc.DepthWriteMask;

    (void) desc.StencilEnable;
    (void) desc.StencilReadMask;
    (void) desc.StencilWriteMask;
  });
  
  desc.DSVFormat              = gx_format(pipeline::format_1_32_float_depth);

  desc.pRootSignature         = root_signature.get();
  desc.NumRenderTargets       = render_outputs.size();

  desc.PrimitiveTopologyType  = gx_primitive(topology_type);

  desc.DS = D3D12_SHADER_BYTECODE{};
  desc.HS = D3D12_SHADER_BYTECODE{};
  desc.GS = D3D12_SHADER_BYTECODE{};

  desc.VS = D3D12_SHADER_BYTECODE{};
  desc.PS = D3D12_SHADER_BYTECODE{};

  for (auto i = 0u; i < render_outputs.size(); i += 1) {

    pipeline::render_output &output = render_outputs[i];

    desc.RTVFormats[i] = gx_format(output.format);

    desc.BlendState.RenderTarget[i] = cx::tell<D3D12_RENDER_TARGET_BLEND_DESC>({}, [&output](D3D12_RENDER_TARGET_BLEND_DESC& blend) {

      blend.BlendEnable           = output.blend.enabled;
      blend.LogicOpEnable         = output.blend.op_enabled;

      blend.SrcBlend              = gx_blend(output.blend.source_blend);
      blend.SrcBlendAlpha         = gx_blend(output.blend.source_alpha_blend);

      blend.DestBlend             = gx_blend(output.blend.destination_blend);
      blend.DestBlendAlpha        = gx_blend(output.blend.destination_alpha_blend);

      blend.BlendOp               = gx_blend_op(output.blend.blend_operation);
      blend.BlendOpAlpha          = gx_blend_op(output.blend.blend_alpha_operation);

      blend.LogicOp               = {};
      blend.RenderTargetWriteMask = output.blend.write_mask;
    });
  }

  context.device->CreateGraphicsPipelineState(&desc, __uuidof(ID3D12PipelineState), pipeline_state.put_void());
}

SNOW_OWL_NAMESPACE_END
