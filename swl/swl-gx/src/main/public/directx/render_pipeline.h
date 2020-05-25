#pragma once

#include <header.hpp>

#include "render_pipeline.hpp"

SNOW_OWL_NAMESPACE(gx::dx)

struct render_pipeline final : gx::render_pipeline {

  render_pipeline   ();

  explicit
    render_pipeline (context &context);


  void
    construct       () override;

  winrt::com_ptr < ID3D12Device > device;

  winrt::com_ptr < ID3D12RootSignature > root_signature;
  winrt::com_ptr < ID3D12PipelineState > pipeline_state;

  size_t item_offset;
  size_t stage_offset;

  winrt::com_ptr < ID3D12DescriptorHeap > descriptor_buf;
};

SNOW_OWL_NAMESPACE_END
