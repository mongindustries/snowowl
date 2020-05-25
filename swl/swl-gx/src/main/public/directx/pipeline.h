#pragma once

#include <d3d12.h>
#include <header.hpp>

#include "pipeline.hpp"

SNOW_OWL_NAMESPACE(gx::dx::pipeline)

DXGI_FORMAT                   gx_format           (gx::pipeline::format const &format);

D3D12_ROOT_PARAMETER_TYPE     gx_param            (gx::pipeline::shader_argument_type const &type);

D3D12_DESCRIPTOR_RANGE_TYPE   gx_param_table      (gx::pipeline::shader_argument_type const &type);

D3D12_PRIMITIVE_TOPOLOGY_TYPE gx_primitive        (gx::pipeline::topology_type const &type);

D3D12_BLEND                   gx_blend            (gx::pipeline::blend_type const &type);

D3D12_BLEND_OP                gx_blend_op         (gx::pipeline::blend_op const &op);

D3D12_TEXTURE_ADDRESS_MODE    gx_address_mode     (gx::pipeline::address_mode const &mode);

D3D12_COMPARISON_FUNC         gx_comparison_func  (gx::pipeline::comparison_type const &type);

D3D12_STENCIL_OP              gx_stencil_op       (gx::pipeline::stencil_op const &type);

SNOW_OWL_NAMESPACE_END
