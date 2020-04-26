//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#include <header.hpp>

#include <window.hpp>

SNOW_OWL_NAMESPACE(gx)

struct graphics_swap_chain;

struct graphics_queue;


struct graphics_context {

	virtual ~graphics_context() = default;

	virtual cx::exp::ptr<graphics_swap_chain>
			create_swap_chain (const cx::exp::ptr_ref<ui::window>& window);

	virtual cx::exp::ptr<graphics_queue>
			create_queue      ();
};

SNOW_OWL_NAMESPACE_END
