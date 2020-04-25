//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#include <vector>

#include <header.hpp>

#include "graphics_context.hpp"
#include "graphics_queue.hpp"

SNOW_OWL_NAMESPACE(gx)

enum class graphics_buffer_type {
	typeData,
	typeTexture2d,
	typeTexture3d
};

template<graphics_buffer_type BufferType = graphics_buffer_type::typeData>
struct graphics_buffer {

	enum class location {
		local,
		host,
		both
	};

	graphics_buffer
				(const cx::exp::ptr_ref<graphics_context>& context, location locality, size_t size) { }

	graphics_buffer
				(const cx::exp::ptr_ref<graphics_context>& context, location locality, const std::vector<char>& buffer) { }


	void  update_data   (unsigned long location, const std::vector<char>& data) {
	}

	void  stage         (const cx::exp::ptr_ref<graphics_queue>& queue) {
	}
};

SNOW_OWL_NAMESPACE_END
