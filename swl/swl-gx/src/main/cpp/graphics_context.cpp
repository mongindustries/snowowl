//
// Created by Michael Ong on 25/4/20.
//
#include "graphics_context.hpp"

#include "graphics_queue.hpp"
#include "graphics_swap_chain.hpp"

SNOW_OWL_NAMESPACE(gx)

cx::exp::ptr<graphics_swap_chain>
graphics_context::create_swap_chain(const cx::exp::ptr_ref<ui::window> &window) {
	return cx::exp::ptr<gx::graphics_swap_chain>{ nullptr };
}

cx::exp::ptr<graphics_queue>
graphics_context::create_queue() {
	return cx::exp::ptr<gx::graphics_queue>{ nullptr };
}

SNOW_OWL_NAMESPACE_END
