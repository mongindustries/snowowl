//
// Created by Michael Ong on 13/4/20.
//
#include "vulkan_queue.hpp"
#include "vulkan_swap_chain.hpp"
#include "vulkan_context.hpp"

using namespace std;
using namespace swl::cx;
using namespace swl::gx;

VulkanGraphicsQueue::VulkanGraphicsQueue(const VulkanGraphicsContext &context, vk::QueueFlagBits type)
: physicalDevice(context._active_device),
	familyIndex(-1),
	ready(false) {

	uint32_t index = 0;
	for (const auto& family : physicalDevice.getQueueFamilyProperties()) {

		if (family.queueFlags & type) {
			familyIndex = index;
			break;
		}

		index += 1;
	}
}

bool VulkanGraphicsQueue::supportsPresent(const VulkanGraphicsSwapChain& swapChain) const {
	return physicalDevice.getSurfaceSupportKHR(familyIndex, swapChain.surface.get());
}

bool VulkanGraphicsQueue::isReady() const {
	return ready;
}

void VulkanGraphicsQueue::prepare(const vk::Device &d) {
	queue = d.getQueue(familyIndex, 0);
	ready = true;

	device = &d;
}


vk::UniqueCommandPool VulkanGraphicsQueue::commandPool() const {

	vk::CommandPoolCreateInfo commandPoolCreate({ vk::CommandPoolCreateFlagBits::eResetCommandBuffer }, familyIndex);
	return device->createCommandPoolUnique(commandPoolCreate);
}

void VulkanGraphicsQueue::submit(const vector<vk::CommandBuffer> &buffers, VulkanGraphicsQueue::GPUWaitType wait, const vk::Fence &fence) const {

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

void VulkanGraphicsQueue::present(const vector<cx::Borrow<VulkanFrame>> &swapChains, GPUWaitType wait) const {

	vector<vk::SwapchainKHR> chains;
	chains.reserve(swapChains.size());

	vector<uint32_t> indices;
	indices.reserve(swapChains.size());

	for (const auto &item : swapChains) {

		auto &frame = item.get();
		auto &sc    = frame.swapChain;

		chains    .emplace_back(sc.swapChain.get());
		indices   .emplace_back(frame.index);
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


VulkanGraphicsQueue::GPUWaitType VulkanGraphicsQueue::GPUWaitType::semaphores(const vector<vk::Semaphore> &wait, const vector<vk::Semaphore> &signal) {
	return VulkanGraphicsQueue::GPUWaitType {false, wait, signal };
}

VulkanGraphicsQueue::GPUWaitType VulkanGraphicsQueue::GPUWaitType::idle() {
	return VulkanGraphicsQueue::GPUWaitType { true };
}
