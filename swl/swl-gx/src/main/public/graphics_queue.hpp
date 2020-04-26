//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#include <vector>

#include <header.hpp>
#include <ownership_exp.hpp>

#include "graphics_context.hpp"

SNOW_OWL_NAMESPACE(gx)

struct graphics_render_block;

struct graphics_queue {

	graphics_queue            (const cx::exp::ptr_ref<graphics_context> &context);

	virtual ~graphics_queue   () = default;


	virtual cx::exp::ptr<graphics_render_block>
				create_render_block ();


	virtual void
				begin               (const std::vector<cx::exp::ptr_ref<graphics_queue>>& dependencies);

	virtual void
				submit              (const std::vector<cx::exp::ptr_ref<graphics_render_block>>& commands);
};

SNOW_OWL_NAMESPACE_END
