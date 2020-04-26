//
// Created by Michael Ong on 26/4/20.
//
#pragma once

#include <vector>

#include <header.hpp>
#include <ownership_exp.hpp>

SNOW_OWL_NAMESPACE(gx)

struct graphics_render_pass;

struct graphics_render_block {

	virtual cx::exp::ptr_ref<graphics_render_pass>
				create_render_pass  ();


	virtual void
				commit_render_pass  ();

	virtual void
				compile             ();


	bool  is_compiled         () const;

protected:

	bool compiled;

	std::vector<cx::exp::ptr<graphics_render_pass>> passes;
};

SNOW_OWL_NAMESPACE_END
