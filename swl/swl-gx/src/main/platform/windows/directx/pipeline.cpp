#include "directx/pipeline.h"

SNOW_OWL_NAMESPACE(gx::dx::pipeline)

DXGI_FORMAT
  gx_format             (gx::pipeline::format const &format) {

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

D3D12_ROOT_PARAMETER_TYPE
  gx_param              (gx::pipeline::shader_argument_type const &type) {
  switch (type) {
  case gx::pipeline::typeConstant:
    return D3D12_ROOT_PARAMETER_TYPE_CBV;
  case gx::pipeline::typeBuffer:
  case gx::pipeline::typeTexture:
    return D3D12_ROOT_PARAMETER_TYPE_SRV;
  }

  return {};
}

D3D12_DESCRIPTOR_RANGE_TYPE
  gx_param_table        (gx::pipeline::shader_argument_type const &type) {
  switch (type) {
  case gx::pipeline::typeConstant:
    return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
  case gx::pipeline::typeBuffer:
  case gx::pipeline::typeTexture:
    return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
  }

  return {};
}

D3D12_PRIMITIVE_TOPOLOGY_TYPE
  gx_primitive          (gx::pipeline::topology_type const &type) {
  switch (type) {
  case gx::pipeline::topologyTypePoint:
    return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
  case gx::pipeline::topologyTypeLine:
    return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
  case gx::pipeline::topologyTypeTriangle:
    return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
  }

  return {};
}

D3D12_BLEND
  gx_blend              (gx::pipeline::blend_type const &type) {

  if (type == gx::pipeline::typeZero) { return D3D12_BLEND_ZERO; }

  if (type == gx::pipeline::typeOne) { return D3D12_BLEND_ONE; }

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

D3D12_BLEND_OP
  gx_blend_op           (gx::pipeline::blend_op const &op) {

  switch (op) {
  case gx::pipeline::opAdd:
    return D3D12_BLEND_OP_ADD;
  case gx::pipeline::opMax:
    return D3D12_BLEND_OP_MAX;
  case gx::pipeline::opMin:
    return D3D12_BLEND_OP_MIN;
  case gx::pipeline::opSubtract:
    return D3D12_BLEND_OP_SUBTRACT;
  }

  return D3D12_BLEND_OP{};
}

D3D12_TEXTURE_ADDRESS_MODE
  gx_address_mode       (gx::pipeline::address_mode const &mode) {
  switch (mode) {
  case gx::pipeline::modeClamp:
    return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
  case gx::pipeline::modeWrap:
    return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
  case gx::pipeline::modeMirror:
    return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
  }
}

D3D12_COMPARISON_FUNC
  gx_comparison_func    (gx::pipeline::comparison_type const &type) {
  switch (type) {
  case gx::pipeline::comparisonAlways:
    return D3D12_COMPARISON_FUNC_ALWAYS;
  case gx::pipeline::comparisonNever:
    return D3D12_COMPARISON_FUNC_NEVER;
  case gx::pipeline::comparisonEqual:
    return D3D12_COMPARISON_FUNC_EQUAL;
  case gx::pipeline::comparisonNotEqual:
    return D3D12_COMPARISON_FUNC_NOT_EQUAL;
  case gx::pipeline::comparisonLess:
    return D3D12_COMPARISON_FUNC_LESS;
  case gx::pipeline::comparisonLessEqual:
    return D3D12_COMPARISON_FUNC_LESS_EQUAL;
  case gx::pipeline::comparisonMore:
    return D3D12_COMPARISON_FUNC_GREATER;
  case gx::pipeline::comparisonMoreEqual:
    return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
  }
}

D3D12_STENCIL_OP
  gx_stencil_op         (gx::pipeline::stencil_op const &type) {
  switch (type) {
  case gx::pipeline::opKeep:
    return D3D12_STENCIL_OP_KEEP;
  case gx::pipeline::opReplace:
    return D3D12_STENCIL_OP_REPLACE;
  case gx::pipeline::opZero:
    return D3D12_STENCIL_OP_ZERO;
  case gx::pipeline::opSatIncrease:
    return D3D12_STENCIL_OP_INCR_SAT;
  case gx::pipeline::opSatDecrease:
    return D3D12_STENCIL_OP_DECR_SAT;
  case gx::pipeline::opInvert:
    return D3D12_STENCIL_OP_INVERT;
  case gx::pipeline::opIncrease:
    return D3D12_STENCIL_OP_INCR;
  case gx::pipeline::opDecrease:
    return D3D12_STENCIL_OP_DECR;
  }
}

SNOW_OWL_NAMESPACE_END
