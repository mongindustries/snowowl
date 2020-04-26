//
// Created by Michael Ong on 25/4/20.
//
#include "graphics_pass.hpp"

SNOW_OWL_NAMESPACE(gx)

graphics_pass::graphics_pass  (
	const cx::exp::ptr_ref<graphics_context>&   context,
	const cx::exp::ptr_ref<graphics_queue>&     queue,
	const cx::exp::ptr_ref<graphics_pipeline>&  pipeline) {

}

graphics_pass::~graphics_pass () {

}


void  graphics_pass::set_viewport
			(const cx::size_2d &value) {

}

void  graphics_pass::set_scissor
			(const cx::rect &value) {

}

void  graphics_pass::bind_buffer
			(graphics_pass::stage_binding binding, unsigned int index, const cx::exp::ptr_ref<graphics_buffer<>> &buffer) {

}

void graphics_pass::bind_fragment_texture
			(unsigned index, const cx::exp::ptr_ref<graphics_buffer<graphics_buffer_type::typeTexture2d>>& texture) {
	
}

void graphics_pass::bind_fragment_texture
			(unsigned index, const cx::exp::ptr_ref<graphics_buffer<graphics_buffer_type::typeTexture3d>>& texture) {
	
}

void  graphics_pass::bind_fragment_sampler
			(unsigned int index, const cx::exp::ptr_ref<graphics_sampler> &sampler) {

}

void  graphics_pass::draw
			(const graphics_pass::draw_range &vertex_range) {

}

void  graphics_pass::draw
			(const graphics_pass::draw_range &index_range, const cx::exp::ptr_ref<graphics_buffer<graphics_buffer_type::typeData>> &buffer) {

}

SNOW_OWL_NAMESPACE_END
