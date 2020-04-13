//
// Created by Michael Ong on 12/4/20.
//
#pragma once

#include <vector>
#include <ownership.hpp>

#include "graphicsSwapChain.h"
#include "rect.hpp"
#include "vulkanImport.h"

SNOW_OWL_NAMESPACE(gx)

struct VulkanGraphicsContext;

struct VulkanGraphicsQueue;

struct VulkanGraphicsSwapChain: GraphicsSwapChain<VulkanGraphicsContext> {

	struct VulkanFrame {

		uint32_t index;

		vk::Image image;
		vk::UniqueImageView imageView;
	};

	vk::PhysicalDevice const&   physicalDevice;
	vk::Device const&           device;
	
	vk::UniqueSurfaceKHR        surface;
	vk::UniqueSwapchainKHR      swapChain;

	vk::UniqueSemaphore         swapChainSemaphore;


	VulkanGraphicsQueue const&  presentQueue;
	VulkanGraphicsQueue const&  graphicsQueue;

	
	std::vector<cx::Own<VulkanFrame>> activeFrames;

	bool needsResize{ false };
	cx::Size2D currentSize;

	VulkanGraphicsSwapChain(
		const VulkanGraphicsContext &context,
		const VulkanGraphicsQueue& graphicsQueue,
		const VulkanGraphicsQueue& presentQueue,
		const ui::WindowSurface &surface);


	[[nodiscard]] cx::Borrow<VulkanFrame>
		getFrame  ();

	void
		createSwapChain();
};

SNOW_OWL_NAMESPACE_END
