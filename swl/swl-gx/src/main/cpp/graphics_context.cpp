//
// Created by Michael Ong on 25/4/20.
//
#include "graphics_context.hpp"

#include "graphics_queue.hpp"
#include "graphics_swap_chain.hpp"

SNOW_OWL_NAMESPACE(gx)

template<typename c>
using ptr_ref = cx::exp::ptr_ref<c>;

cx::exp::ptr<graphics_swap_chain>
			graphics_context::create_swap_chain   (const ptr_ref<ui::window> &window, const ptr_ref<gx::graphics_queue>& present_queue) {
	return cx::exp::ptr<gx::graphics_swap_chain>{ new graphics_swap_chain(ptr_ref<graphics_context>{ this }, present_queue, window) };
}

cx::exp::ptr<graphics_queue>
			graphics_context::create_queue        () {
	return cx::exp::ptr<gx::graphics_queue>{ new graphics_queue(ptr_ref<graphics_context>{ this }) };
}

cx::exp::ptr<graphics_buffer_allocator>
			graphics_context::create_allocator    () {
	return cx::exp::ptr<graphics_buffer_allocator>{ nullptr };
}

SNOW_OWL_NAMESPACE_END
