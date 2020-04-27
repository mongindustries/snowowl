//
// Created by Michael Ong on 26/4/20.
//
#pragma once

#include <vector>
#include <functional>

#include <header.hpp>
#include <ownership_exp.hpp>

SNOW_OWL_NAMESPACE(gx)

struct graphics_render_pass;

struct graphics_render_block {


	void  render_pass  (const std::function<void(graphics_render_pass&)> &configure);


	void  begin        ();

	void  end          ();
};

SNOW_OWL_NAMESPACE_END
