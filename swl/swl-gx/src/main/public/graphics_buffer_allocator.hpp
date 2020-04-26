//
// Created by Michael Ong on 26/4/20.
//
#pragma once

#include <header.hpp>
#include <ownership_exp.hpp>
#include "graphics_context.hpp"
#include "graphics_buffer.hpp"

SNOW_OWL_NAMESPACE(gx)

struct graphics_buffer_allocator {

	graphics_buffer_allocator
										(cx::exp::ptr_ref<graphics_context> const& context);


	virtual void
			allocate      (cx::exp::ptr_ref<graphics_buffer<graphics_buffer_type::typeData>> const& buffer);

	virtual void
			allocate      (cx::exp::ptr_ref<graphics_buffer<graphics_buffer_type::typeTexture2d>> const& buffer);

	virtual void
			allocate      (cx::exp::ptr_ref<graphics_buffer<graphics_buffer_type::typeTexture3d>> const& buffer);
};

SNOW_OWL_NAMESPACE_END
