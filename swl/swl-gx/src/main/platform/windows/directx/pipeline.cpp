#include "directx/pipeline.h"

SNOW_OWL_NAMESPACE(gx::dx)

pipeline::pipeline(
	const cx::exp::ptr_ref<dx::context>& context,
	std::array<graphics_shader, 2>              shader_stages,
	gx::pipeline::raster                        raster,
	gx::pipeline::depth                         depth,
	gx::pipeline::stencil                       stencil,
	gx::pipeline::sample                        sample,
	bool                                        independent_blend,
	gx::pipeline::topology_type                 topology_type,
	std::array<gx::pipeline::render_output, 8>  render_outputs) :

	graphics_render_pipeline(shader_stages, raster, depth, stencil, sample, independent_blend, topology_type, render_outputs) {

	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};

	desc.NumRenderTargets = render_outputs.size();

	context->device->CreateGraphicsPipelineState(&desc, __uuidof(ID3D12PipelineState), pipeline_state.put_void());
}

SNOW_OWL_NAMESPACE_END
