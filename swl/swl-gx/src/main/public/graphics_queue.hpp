//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#include <vector>

#include <header.hpp>
#include <ownership_exp.hpp>

#include "graphics_context.hpp"

SNOW_OWL_NAMESPACE(gx)

struct graphics_pass;

struct graphics_queue {

	graphics_queue           (const cx::exp::ptr_ref<graphics_context> &context);

	virtual ~graphics_queue  () = default;


	virtual void  begin   ();

	virtual void  submit  (const std::vector<cx::exp::ptr_ref<graphics_pass>>& commands);
};

SNOW_OWL_NAMESPACE_END
