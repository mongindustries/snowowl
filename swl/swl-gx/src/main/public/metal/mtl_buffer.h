//
// Created by Michael Ong on 26/4/20.
//
#pragma once

#include <header.hpp>
#include <graphics_buffer.hpp>
#import "mtl_context.h"

SNOW_OWL_NAMESPACE(gx::mtl)

struct mtl_buffer: graphics_buffer<graphics_buffer_type::typeData> {

	mtl_buffer
		(const cx::exp::ptr_ref<mtl_context>& context, location locality, size_t size);

	mtl_buffer
		(const cx::exp::ptr_ref<mtl_context>& context, location locality, const std::vector<char>& buffer);


	void update_data  (unsigned long start, const std::vector<char> &data) override;

	void stage        (const cx::exp::ptr_ref<graphics_queue> &staging_queue) override;


	id<MTLBuffer>             buffer;

	id<MTLBuffer> __nullable  staging_buffer;
};

SNOW_OWL_NAMESPACE_END
