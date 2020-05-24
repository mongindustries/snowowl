#pragma once

#include <header.hpp>

#include "render_pipeline.hpp"

#include "directx/context.h"

SNOW_OWL_NAMESPACE(gx::dx)

struct render_pipeline final : gx::render_pipeline {

  render_pipeline();

  explicit
    render_pipeline(context &context);


  void
    construct() override;


  winrt::com_ptr < ID3D12Device > device;

  winrt::com_ptr < ID3D12RootSignature > root_signature;
  winrt::com_ptr < ID3D12PipelineState > pipeline_state;

  winrt::com_ptr < ID3D12DescriptorHeap > descriptor_buf;
  winrt::com_ptr < ID3D12DescriptorHeap > descriptor_rtv;
  winrt::com_ptr < ID3D12DescriptorHeap > descriptor_dsv;
};

SNOW_OWL_NAMESPACE_END
