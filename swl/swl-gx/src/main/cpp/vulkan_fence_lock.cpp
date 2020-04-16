//
// Created by Michael Ong on 16/4/20.
//
#include <array>
#include "vulkan_fence_lock.hpp"

using namespace std;

swl::gx::VulkanFenceLock::VulkanFenceLock(const vk::Device& device, const vk::Fence &fence): device(device), fence(fence) {
	device.resetFences(array{ fence });
}

swl::gx::VulkanFenceLock::~VulkanFenceLock() {
	device.waitForFences(array{ fence }, true, UINT64_MAX);
}
