//
// Created by Michael Ong on 13/4/20.
//
#pragma once

#include <headerconv.hpp>
#include <ownership.hpp>

#include "vulkanImport.h"
#include "vulkanGraphicsSwapChain.hpp"

SNOW_OWL_NAMESPACE(gx)

struct VulkanGraphicsContext;

struct VulkanGraphicsQueue {

	struct WaitType {
		static WaitType semaphores(const std::vector<vk::Semaphore> &wait, const std::vector<vk::Semaphore> &signal);
		static WaitType idle();

		bool shouldIdle{false};

		std::vector<vk::Semaphore> wait;
		std::vector<vk::Semaphore> signal;
	};


	vk::PhysicalDevice const& physicalDevice;

	vk::Device const* device{};


	vk::Queue queue;

	uint32_t familyIndex;

	bool ready;


	VulkanGraphicsQueue(const VulkanGraphicsContext&, vk::QueueFlagBits type);

	void prepare(const vk::Device& d);


	[[nodiscard]] bool supportsPresent(const VulkanGraphicsSwapChain& swapChain) const;

	bool isReady() const;


	[[nodiscard]] vk::UniqueCommandPool commandPool() const;


	void submit(const std::vector<vk::CommandBuffer> &buffers, WaitType wait) const;

	void present(const std::vector<std::pair<cx::Borrow<VulkanGraphicsSwapChain>, cx::Borrow<VulkanGraphicsSwapChain::Frame>>> &swapChains, WaitType wait) const;
};

SNOW_OWL_NAMESPACE_END
