//
// Created by Michael Ong on 25/4/20.
//
#include "graphics_queue.hpp"

SNOW_OWL_NAMESPACE(gx)

graphics_queue::graphics_queue
			(const cx::exp::ptr_ref<gx::graphics_context> &context) { }


void  graphics_queue::begin   () { }

void  graphics_queue::submit  (const std::vector<cx::exp::ptr_ref<graphics_pass>>& commands) { }

SNOW_OWL_NAMESPACE_END
