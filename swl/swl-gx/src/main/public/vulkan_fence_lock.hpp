//
// Created by Michael Ong on 16/4/20.
//
#pragma once

#include <header.hpp>

#include "vulkanImport.h"

SNOW_OWL_NAMESPACE(gx)

struct VulkanFenceLock {

	vk::Fence fence;

	vk::Device const& device;

	VulkanFenceLock(const vk::Device& device, const vk::Fence &fence);

	~VulkanFenceLock();
};

SNOW_OWL_NAMESPACE_END
