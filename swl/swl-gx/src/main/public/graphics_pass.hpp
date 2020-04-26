//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#include <header.hpp>
#include <rect.hpp>

#include "graphics_context.hpp"
#include "graphics_queue.hpp"
#include "graphics_pipeline.hpp"
#include "graphics_buffer.hpp"
#include "graphics_sampler.hpp"

SNOW_OWL_NAMESPACE(gx)

struct graphics_pass {

	enum class stage_binding {
		bindingGraphicsVertex,
		bindingGraphicsFragment,
	};

	struct draw_range {
		unsigned int begin;
		unsigned int size;
	};

	graphics_pass   (
		const cx::exp::ptr_ref<graphics_context>&   context ,
		const cx::exp::ptr_ref<graphics_queue>&     queue   ,
		const cx::exp::ptr_ref<graphics_pipeline>&  pipeline);

	~graphics_pass  ();


	void  set_viewport          (const cx::size_2d& value);

	void  set_scissor           (const cx::rect& value);


	void  bind_buffer           (stage_binding binding, unsigned int index, const cx::exp::ptr_ref<graphics_buffer<graphics_buffer_type::typeData>>& buffer);


	void  bind_fragment_texture (unsigned int index, const cx::exp::ptr_ref<graphics_buffer<graphics_buffer_type::typeTexture2d>>& texture);

	void  bind_fragment_texture (unsigned int index, const cx::exp::ptr_ref<graphics_buffer<graphics_buffer_type::typeTexture3d>>& texture);
	
	void  bind_fragment_sampler (unsigned int index, const cx::exp::ptr_ref<graphics_sampler>& sampler);


	void  draw                  (const draw_range& vertex_range);

	void  draw                  (const draw_range& index_range, const cx::exp::ptr_ref<graphics_buffer<graphics_buffer_type::typeData>>& buffer);
};

SNOW_OWL_NAMESPACE_END
