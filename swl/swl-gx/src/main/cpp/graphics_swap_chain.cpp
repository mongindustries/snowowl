//
// Created by Michael Ong on 25/4/20.
//
#include "graphics_swap_chain.hpp"

SNOW_OWL_NAMESPACE(gx)

graphics_swap_chain::graphics_swap_chain
			(const cx::exp::ptr_ref<graphics_context> &ctx,
			 const cx::exp::ptr_ref<graphics_queue>& queue,
			 const cx::exp::ptr_ref<ui::window> &window): window(window) { }

cx::exp::ptr_ref<graphics_swap_chain::frame>
			graphics_swap_chain::next_frame () { return cx::exp::ptr_ref<frame> { nullptr }; }

void  graphics_swap_chain::present    () { }

void  graphics_swap_chain::resize     (const cx::size_2d& new_size) { }

SNOW_OWL_NAMESPACE_END
