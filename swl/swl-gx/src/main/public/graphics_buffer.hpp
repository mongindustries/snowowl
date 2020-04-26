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

template<graphics_buffer_type BufferType>
struct graphics_buffer {
};

template<>
struct graphics_buffer<graphics_buffer_type::typeData> {

	enum class location {
		local,
		host,
		both
	};

	graphics_buffer
		(const cx::exp::ptr_ref<graphics_context>& context, location locality, size_t size) { }

	graphics_buffer
		(const cx::exp::ptr_ref<graphics_context>& context, location locality, const std::vector<char>& buffer) { }

	~graphics_buffer() = default;


	virtual void  update_data   (unsigned long start, const std::vector<char>& data) {
	}

	virtual void  stage         (const cx::exp::ptr_ref<graphics_queue>& staging_queue) {
	}
};

template<>
struct graphics_buffer<graphics_buffer_type::typeTexture2d> {

	enum class location {
		local,
		host,
		both
	};

	graphics_buffer
		(const cx::exp::ptr_ref<graphics_context>& context, location locality, cx::size_2d size, int format, const std::vector<char>& data) { }


	virtual void  update_data   (cx::rect frame, const std::vector<char>& data) {
	}

	virtual void  stage         (const cx::exp::ptr_ref<graphics_queue>& staging_queue) {
	}
};

SNOW_OWL_NAMESPACE_END
