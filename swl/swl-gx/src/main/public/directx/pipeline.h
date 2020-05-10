#pragma once

#include <header.hpp>

#include "render_pipeline.hpp"

#include "directx/context.h"

SNOW_OWL_NAMESPACE(gx::dx)

struct render_pipeline final : gx::render_pipeline {

  render_pipeline           ();

  explicit render_pipeline  (context& context);

  winrt::com_ptr<ID3D12RootSignature> root_signature;
  winrt::com_ptr<ID3D12PipelineState> pipeline_state;
};

SNOW_OWL_NAMESPACE_END
