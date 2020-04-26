//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#include <header.hpp>

#include <window.hpp>

SNOW_OWL_NAMESPACE(gx)

struct graphics_swap_chain;

struct graphics_queue;

struct graphics_buffer_allocator;


struct graphics_context {

	virtual ~graphics_context
												() = default;

	virtual cx::exp::ptr<graphics_swap_chain>
			create_swap_chain (const cx::exp::ptr_ref<ui::window>& window, const cx::exp::ptr_ref<gx::graphics_queue>& present_queue);

	virtual cx::exp::ptr<graphics_queue>
			create_queue      ();

	virtual cx::exp::ptr<graphics_buffer_allocator>
			create_allocator  ();
};

SNOW_OWL_NAMESPACE_END
