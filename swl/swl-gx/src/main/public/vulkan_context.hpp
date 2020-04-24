//
// Created by Michael Ong on 6/4/20.
//
#pragma once

#include <header.hpp>
#include <ownership_exp.hpp>

#include <windowSurface.hpp>

#include "vulkan_import.h"
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
		create_device  (const std::vector<cx::exp::ptr_ref<VulkanGraphicsQueue>> &queues);


	vk::UniqueInstance  _instance;

	vk::PhysicalDevice  _active_device;

	vk::UniqueDevice    _device;

private:

	vk::DynamicLoader*  loader;
};

SNOW_OWL_NAMESPACE_END
