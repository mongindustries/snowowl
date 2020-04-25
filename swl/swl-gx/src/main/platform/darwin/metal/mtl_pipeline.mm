//
// Created by Michael Ong on 25/4/20.
//
#include "metal/mtl_pipeline.h"

SNOW_OWL_NAMESPACE(gx::mtl)

MTLBlendFactor convert_blend(pipeline::blend_type type) {

	MTLBlendOperation;

	switch (type) {
		case pipeline::blend_type::typeZero:
			return MTLBlendFactorZero;
		case pipeline::blend_type::typeOne:
			return MTLBlendFactorZero;
		case pipeline::blend_type::typeSourceColor:
			return MTLBlendFactorZero;
		case pipeline::blend_type::typeSourceColorInvert:
			return MTLBlendFactorZero;
		case pipeline::blend_type::typeSourceAlpha:
			return MTLBlendFactorZero;
		case pipeline::blend_type::typeSourceAlphaInvert:
			return MTLBlendFactorZero;
		case pipeline::blend_type::typeDestinationColor:
			return MTLBlendFactorZero;
		case pipeline::blend_type::typeDestinationColorInvert:
			return MTLBlendFactorZero;
		case pipeline::blend_type::typeDestinationAlpha:
			return MTLBlendFactorZero;
		case pipeline::blend_type::typeDestinationAlphaInvert:
			return MTLBlendFactorZero;
		case pipeline::blend_type::typeBlendFactor:
			return MTLBlendFactorZero;
		case pipeline::blend_type::typeBlendFactorInvert:
			return MTLBlendFactorZero;
	}
}

MTLBlendOperation convert_blend_op(pipeline::blend_op op) {
	switch (op) {
	case pipeline::blend_op::opAdd:
		return MTLBlendOperationAdd;
	case pipeline::blend_op::opSubtract:
		return MTLBlendOperationSubtract;
	case pipeline::blend_op::opMin:
		return MTLBlendOperationMin;
	case pipeline::blend_op::opMax:
		return MTLBlendOperationMax;
	}
}

mtl_pipeline::mtl_pipeline(
	const cx::exp::ptr_ref<mtl_context>&    context,
	const cx::exp::ptr_ref<mtl_queue>&      queue,
	std::array<graphics_shader, 2>          shader_stages,
	swl::gx::pipeline::raster               raster,
	swl::gx::pipeline::depth                depth,
	swl::gx::pipeline::stencil              stencil,
	swl::gx::pipeline::sample               sample,
	bool                                    independent_blend,
	swl::gx::pipeline::topology_type        topology_type,
	std::array<pipeline::render_output, 8>  render_outputs) :
	graphics_pipeline(
		shader_stages,
		raster,
		depth,
		stencil,
		sample,
		independent_blend,
		topology_type,
		render_outputs) {

	MTLRenderPipelineDescriptor* descriptor = [MTLRenderPipelineDescriptor new];

	descriptor.rasterizationEnabled = YES;

	uint16_t index{0};
	for(auto& output : render_outputs) {

		auto obj = [descriptor.colorAttachments objectAtIndexedSubscript:index];

		obj.blendingEnabled             = output.blend.enabled;

		obj.rgbBlendOperation           = convert_blend_op(output.blend.blend_operation);
		obj.alphaBlendOperation         = convert_blend_op(output.blend.blend_alpha_operation);

		obj.sourceRGBBlendFactor        = convert_blend(output.blend.source_blend);
		obj.sourceAlphaBlendFactor      = convert_blend(output.blend.source_alpha_blend);

		obj.destinationRGBBlendFactor   = convert_blend(output.blend.destination_blend);
		obj.destinationAlphaBlendFactor = convert_blend(output.blend.destination_alpha_blend);

		obj.writeMask                   = MTLColorWriteMaskNone;
		if (output.blend.write_mask & pipeline::write_mask_r) {
			obj.writeMask |= MTLColorWriteMaskRed;
		}
		if (output.blend.write_mask & pipeline::write_mask_g) {
			obj.writeMask |= MTLColorWriteMaskGreen;
		}
		if (output.blend.write_mask & pipeline::write_mask_b) {
			obj.writeMask |= MTLColorWriteMaskBlue;
		}
		if (output.blend.write_mask & pipeline::write_mask_a) {
			obj.writeMask |= MTLColorWriteMaskAlpha;
		}

		index += 1;
	}
}

SNOW_OWL_NAMESPACE_END
