//
// Created by Michael Ong on 25/4/20.
//
#include "graphics_render_pass.hpp"

SNOW_OWL_NAMESPACE(gx)

template<typename c>
using ptr_ref = cx::exp::ptr_ref<c>;

graphics_render_pass::graphics_render_pass  () {

}

graphics_render_pass::~graphics_render_pass () {

}


void  graphics_render_pass::set_viewport
			(const cx::size_2d &value) {

}

void  graphics_render_pass::set_scissor
			(const cx::rect &value) {

}

void  graphics_render_pass::bind_buffer
			(graphics_render_pass::stage_binding binding, unsigned int index, const ptr_ref<graphics_buffer<graphics_buffer_type::typeData>> &buffer) {

}

void  graphics_render_pass::bind_fragment_texture
			(unsigned index, const ptr_ref<graphics_buffer<graphics_buffer_type::typeTexture2d>>& texture) {
	
}

void  graphics_render_pass::bind_fragment_texture
			(unsigned index, const ptr_ref<graphics_buffer<graphics_buffer_type::typeTexture3d>>& texture) {
	
}

void  graphics_render_pass::bind_fragment_sampler
			(unsigned int index, const ptr_ref<graphics_sampler> &sampler) {

}

void  graphics_render_pass::draw
			(const graphics_render_pass::draw_range &vertex_range) {

}

void  graphics_render_pass::draw
			(const graphics_render_pass::draw_range &index_range, const ptr_ref<graphics_buffer<graphics_buffer_type::typeData>> &buffer) {

}

SNOW_OWL_NAMESPACE_END
