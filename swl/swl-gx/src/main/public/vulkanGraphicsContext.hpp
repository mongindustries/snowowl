//
// Created by Michael Ong on 6/4/20.
//
#pragma once

#include <header.hpp>
#include <windowSurface.hpp>

#include "vulkanImport.h"
#include "graphics_context.hpp"

SNOW_OWL_NAMESPACE(gx)

struct VulkanGraphicsQueue;

struct VulkanGraphicsContext final: GraphicsContext<VulkanGraphicsContext> {


	VulkanGraphicsContext
		(VulkanGraphicsContext&& mov) noexcept;

	VulkanGraphicsContext& operator=
		(VulkanGraphicsContext&& mov) noexcept;


	VulkanGraphicsContext();

	void
		createDevice  (const std::vector<cx::MutableBorrow<VulkanGraphicsQueue>> &queues);


	vk::UniqueInstance  _instance;

	vk::PhysicalDevice  _active_device;

	vk::UniqueDevice    _device;

private:

	vk::DynamicLoader*  loader;
};

SNOW_OWL_NAMESPACE_END
