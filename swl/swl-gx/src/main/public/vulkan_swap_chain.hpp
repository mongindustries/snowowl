//
// Created by Michael Ong on 12/4/20.
//
#pragma once

#include <vector>

#include <ownership_exp.hpp>
#include <rect.hpp>

#include <event.hpp>

#include "graphics_swap_chain.h"
#include "vulkan_import.h"

SNOW_OWL_NAMESPACE(gx)

struct VulkanGraphicsContext;

struct VulkanGraphicsQueue;

struct VulkanGraphicsSwapChain;

struct VulkanFrame {

	uint32_t                       index;

	vk::Image                      image;
	vk::UniqueImageView            image_view;

	vk::Format                     format;

	VulkanGraphicsSwapChain const& swap_chain;
};

struct VulkanGraphicsSwapChain: GraphicsSwapChain<VulkanGraphicsContext> {

	vk::PhysicalDevice const&           physical_device;
	vk::Device const&                   device;

	vk::UniqueSurfaceKHR                surface;
	vk::UniqueSwapchainKHR              swap_chain;

	vk::UniqueSemaphore                 semaphore;

	VulkanGraphicsQueue const&          queue_present;
	VulkanGraphicsQueue const&          queue_graphics;

	bool                                needs_resize{ false };
	cx::size_2d                          current_size;

	vk::Format                          format;

	std::vector<cx::exp::ptr<VulkanFrame>> active_frames;

	std::vector<ui::Event<void>>        recreate_size_events;

	VulkanGraphicsSwapChain(
		const VulkanGraphicsContext &context,
		const VulkanGraphicsQueue& graphicsQueue,
		const VulkanGraphicsQueue& presentQueue,
		const ui::WindowSurface &surface);


	[[nodiscard]] cx::exp::ptr_ref<VulkanFrame>
		getFrame  ();

	void
		createSwapChain();
};

SNOW_OWL_NAMESPACE_END
