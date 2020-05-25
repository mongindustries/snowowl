#pragma once

#include <header.hpp>

#include "pipeline.hpp"
#include "render_pass.hpp"

SNOW_OWL_NAMESPACE(gx::dx)

struct render_block;

struct render_pipeline;


struct render_pass final : gx::render_pass {

  render_pass   (dx::render_block &block, std::array < pipeline::pass_output, 8 > const &outputs);

  ~render_pass  ();


  void
    topology        (topology_type type) override;

  cx::exp::ptr < gx::render_pass_resource_scope >
    buffer_prepare  (std::array < gx::pipeline::pass_input, NBS > const &transitions) override;

  void
    buffer_bind     (pipeline::shader_stage const &binding, int slot, cx::exp::ptr_ref < gx::resource_reference > const &reference) override;

  void
    buffer_bind     (pipeline::shader_stage const &binding, std::array < cx::exp::ptr_ref < gx::resource_reference >, NBS > const &references) override;

  void
    draw            (pipeline::draw_range const &vertex_range) override;


  winrt::com_ptr < ID3D12GraphicsCommandList4 > command_list;

  std::vector < D3D12_RESOURCE_BARRIER > to_barriers;

  cx::exp::ptr_ref< dx::render_pipeline > pipeline;
};

SNOW_OWL_NAMESPACE_END
