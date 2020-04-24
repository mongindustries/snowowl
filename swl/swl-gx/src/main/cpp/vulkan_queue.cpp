//
// Created by Michael Ong on 13/4/20.
//
#include "vulkan_queue.hpp"
#include "vulkan_swap_chain.hpp"
#include "vulkan_context.hpp"

using namespace std;
using namespace swl;

gx::VulkanGraphicsQueue::VulkanGraphicsQueue(
	const VulkanGraphicsContext&  context,
	vk::QueueFlagBits             type): p_device(context._active_device), family_index(-1), ready(false) {

	uint32_t index = 0;
	for (const auto& family : p_device.getQueueFamilyProperties()) {

		if (family.queueFlags & type) {
			family_index = index;
			break;
		}

		index += 1;
	}
}

bool    gx::VulkanGraphicsQueue::present_support  (const VulkanGraphicsSwapChain& swapChain) const {
	return p_device.getSurfaceSupportKHR(family_index, swapChain.surface.get());
}

bool    gx::VulkanGraphicsQueue::is_ready         () const {
	return ready;
}

void    gx::VulkanGraphicsQueue::prepare          (const vk::Device &d) {
	queue = d.getQueue(family_index, 0);
	ready = true;

	device = &d;
}


vk::UniqueCommandPool
				gx::VulkanGraphicsQueue::commandPool      () const {

	vk::CommandPoolCreateInfo commandPoolCreate({ vk::CommandPoolCreateFlagBits::eResetCommandBuffer }, family_index);
	return device->createCommandPoolUnique(commandPoolCreate);
}

void    gx::VulkanGraphicsQueue::submit           (
	const std::vector<vk::CommandBuffer>& buffers,
	VulkanGraphicsQueue::GPUWaitType      wait,
	const vk::Fence&                      fence) const {

	auto waitFlags = vector<vk::PipelineStageFlags> {
		vk::PipelineStageFlagBits::eTransfer,
		vk::PipelineStageFlagBits::eVertexShader,
		vk::PipelineStageFlagBits::eFragmentShader
	};

	array<vk::SubmitInfo, 1> submitInfo {
		vk::SubmitInfo(
			wait.wait.size(), wait.wait.data(),
			waitFlags.data(),
			buffers.size(), buffers.data(),
			wait.signal.size(), wait.signal.data())
	};

	try {
		queue.submit(submitInfo, fence);

		if (wait.shouldIdle) {
			queue.waitIdle();
		}
	} catch(const vk::SystemError&) {
	}
}

void    gx::VulkanGraphicsQueue::present          (
	const std::vector<cx::exp::ptr_ref<VulkanFrame>>& swapChains,
	GPUWaitType                                       wait) const {

	std::vector<vk::SwapchainKHR> chains;
	chains.reserve(swapChains.size());

	std::vector<uint32_t> indices;
	indices.reserve(swapChains.size());

	for (const VulkanFrame &item : swapChains) {

		chains    .emplace_back(item.swap_chain.swap_chain.get());
		indices   .emplace_back(item.index);
	}

	vk::PresentInfoKHR presentInfo{};

	presentInfo.swapchainCount      = swapChains.size();

	presentInfo.pSwapchains         = chains.data();
	presentInfo.pImageIndices       = indices.data();

	presentInfo.waitSemaphoreCount  = wait.wait.size();
	presentInfo.pWaitSemaphores     = wait.wait.data();

	try {
		(void) queue.presentKHR(presentInfo);

		if (wait.shouldIdle) {
			queue.waitIdle();
		}
	} catch(const vk::SystemError&) {
	}
}


gx::VulkanGraphicsQueue::GPUWaitType
	gx::VulkanGraphicsQueue::GPUWaitType::semaphores(
		const vector<vk::Semaphore> &wait,
		const vector<vk::Semaphore> &signal) {
	return VulkanGraphicsQueue::GPUWaitType {false, wait, signal };
}

gx::VulkanGraphicsQueue::GPUWaitType
	gx::VulkanGraphicsQueue::GPUWaitType::idle      () {
	return VulkanGraphicsQueue::GPUWaitType { true };
}
