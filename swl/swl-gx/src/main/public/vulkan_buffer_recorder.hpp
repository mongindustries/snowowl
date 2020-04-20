//
// Created by Michael Ong on 16/4/20.
//
#pragma once

#include <header.hpp>

#include "vulkan_import.h"

SNOW_OWL_NAMESPACE(gx)

struct VulkanBufferRecorder {

	vk::CommandBuffer buffer;

	VulkanBufferRecorder(const vk::CommandBuffer &buffer, const vk::CommandBufferUsageFlags& usageFlags): buffer(buffer) {
		buffer.begin(vk::CommandBufferBeginInfo(usageFlags));
	}

	~VulkanBufferRecorder() {
		buffer.end();
	}


	vk::CommandBuffer const* operator->() const {
		return &buffer;
	}
};

SNOW_OWL_NAMESPACE_END
