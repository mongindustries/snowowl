//
// Created by Michael Ong on 25/4/20.
//
#include "graphics_render_pipeline.hpp"

SNOW_OWL_NAMESPACE(gx)

pipeline::shader_stage::operator uint16_t() const {
	return value;
}

pipeline::write_mask::operator uint16_t() const {
	return value;
}

bool pipeline::write_mask::operator&(const pipeline::write_mask &rhs) {
	return (value & rhs.value) > 0;
}

pipeline::write_mask pipeline::write_mask::r = pipeline::write_mask{ 0b0001 };
pipeline::write_mask pipeline::write_mask::g = pipeline::write_mask{ 0b0010 };
pipeline::write_mask pipeline::write_mask::b = pipeline::write_mask{ 0b0100 };
pipeline::write_mask pipeline::write_mask::a = pipeline::write_mask{ 0b1000 };

pipeline::shader_stage pipeline::shader_stage::vertex   = pipeline::shader_stage{ 0 };
pipeline::shader_stage pipeline::shader_stage::fragment = pipeline::shader_stage{ 1 };

graphics_render_pipeline::graphics_render_pipeline(
	std::array<graphics_shader, 2>          shader_stages,
	swl::gx::pipeline::raster               raster,
	swl::gx::pipeline::depth                depth,
	swl::gx::pipeline::stencil              stencil,
	swl::gx::pipeline::sample               sample,
	bool                                    independent_blend,
	swl::gx::pipeline::topology_type        topology_type,
	std::array<pipeline::render_output, 8>  render_outputs) { }

SNOW_OWL_NAMESPACE_END
