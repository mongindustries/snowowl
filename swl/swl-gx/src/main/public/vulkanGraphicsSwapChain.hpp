//
// Created by Michael Ong on 12/4/20.
//
#pragma once

#include <vector>
#include <ownership.hpp>

#include "graphicsSwapChain.h"
#include "vulkanImport.h"

SNOW_OWL_NAMESPACE(gx)

struct VulkanGraphicsContext;

struct VulkanGraphicsQueue;

struct VulkanGraphicsSwapChain: GraphicsSwapChain<VulkanGraphicsContext> {

	struct VulkanFrame: Frame {
		vk::Image image;
		vk::UniqueImageView imageView;
	};

	vk::Device const&           device;

	vk::UniqueSurfaceKHR        surface;
	vk::UniqueSwapchainKHR      swapChain;

	vk::UniqueSemaphore         swapChainSemaphore;

	std::vector<cx::Own<Frame>> activeFrames;

	VulkanGraphicsSwapChain(
		const VulkanGraphicsContext &context,
		const VulkanGraphicsQueue& graphicsQueue,
		const VulkanGraphicsQueue& presentQueue,
		const ui::WindowSurface &surface);


	[[nodiscard]] cx::Borrow<Frame>
		getFrame  () const override;
};

SNOW_OWL_NAMESPACE_END
