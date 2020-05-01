//
// Created by Michael Ong on 25/4/20.
//
#include "graphics_queue.hpp"

#include "graphics_render_pipeline.hpp"
#include "graphics_render_block.hpp"

SNOW_OWL_NAMESPACE(gx)

template<typename c>
using ptr_ref = cx::exp::ptr_ref<c>;

graphics_queue::graphics_queue
			(const ptr_ref<gx::graphics_context> &context): context(context) { }


cx::exp::ptr<graphics_render_block>
			graphics_queue::create_render_block (const ptr_ref<graphics_render_pipeline>& pipeline) {
	return cx::exp::ptr<graphics_render_block>{ new graphics_render_block(cx::exp::ptr_ref{ this }, pipeline) };
}


void  graphics_queue::begin               (const std::vector<ptr_ref<graphics_queue>>& dependencies) { }

void  graphics_queue::submit              (const std::vector<ptr_ref<graphics_render_block>>& commands) { }

SNOW_OWL_NAMESPACE_END
