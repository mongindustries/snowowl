#pragma once

#include <header.hpp>

#include "graphics_render_pipeline.hpp"

#include "directx/context.h"

SNOW_OWL_NAMESPACE(gx::dx)

struct render_pipeline : graphics_render_pipeline {

  render_pipeline(const cx::exp::ptr_ref<dx::context>& context);


  winrt::com_ptr<ID3D12PipelineState> pipeline_state;
};

SNOW_OWL_NAMESPACE_END
