//
// Created by Michael Ong on 13/4/20.
//
#pragma once

#include <header.hpp>
#include <ownership.hpp>

#include "vulkan_import.h"

SNOW_OWL_NAMESPACE(gx)

struct VulkanFrame;

struct VulkanGraphicsContext;

struct VulkanGraphicsSwapChain;

struct VulkanGraphicsQueue {

	struct GPUWaitType {

		static GPUWaitType semaphores
			(const std::vector<vk::Semaphore> &wait, const std::vector<vk::Semaphore> &signal);

		static GPUWaitType idle
			();

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

	[[nodiscard]] bool isReady() const;


	[[nodiscard]] vk::UniqueCommandPool commandPool() const;


	void submit(const std::vector<vk::CommandBuffer> &buffers, GPUWaitType wait, const vk::Fence &fence) const;

	void present(
		const std::vector<cx::Borrow<VulkanFrame>> &swapChains,
		GPUWaitType wait) const;
};

SNOW_OWL_NAMESPACE_END