//
// Created by Michael Ong on 16/4/20.
//
#pragma once

#include <header.hpp>

#include "vulkan_import.h"

SNOW_OWL_NAMESPACE(gx)

struct VulkanBufferRecord {

	vk::CommandBuffer buffer;

	VulkanBufferRecord(const vk::CommandBuffer &buffer, const vk::CommandBufferUsageFlags& usageFlags): buffer(buffer) {
		buffer.begin(vk::CommandBufferBeginInfo(usageFlags));
	}

	~VulkanBufferRecord() {
		buffer.end();
	}


	vk::CommandBuffer const* operator->() const {
		return &buffer;
	}
};

SNOW_OWL_NAMESPACE_END
