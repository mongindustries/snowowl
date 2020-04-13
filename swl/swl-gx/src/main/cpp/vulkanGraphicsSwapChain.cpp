//
// Created by Michael Ong on 12/4/20.
//
#include <algorithm>
#include <path_ops.hpp>

#include "vulkanGraphicsSwapChain.hpp"

#include "application.hpp"
#include "application.hpp"
#include "vulkanGraphicsContext.hpp"
#include "vulkanGraphicsBackend.hpp"

#include "window.hpp"

#undef min
#undef max

using namespace swl::cx;
using namespace swl::gx;
using namespace swl::ui;

vk::Extent2D getExtent(const Size2D &surface, const vk::SurfaceCapabilitiesKHR &capabilities);

VulkanGraphicsSwapChain::VulkanGraphicsSwapChain(
	const VulkanGraphicsContext &context,
	const VulkanGraphicsQueue& graphicsQueue,
	const VulkanGraphicsQueue& presentQueue,
	const swl::ui::WindowSurface &surface) : GraphicsSwapChain(context, surface),

	physicalDevice (context._active_device),
	device         (context._device.get()),

	surface (backend::VulkanGraphicsBackend::instance->makeSurface(context._instance.get(), surface)),

	presentQueue  (presentQueue),
	graphicsQueue (graphicsQueue),

	currentSize(surface.getSize()) {

	const vk::SemaphoreCreateInfo createSemaphore;
	swapChainSemaphore = device.createSemaphoreUnique(createSemaphore);

	createSwapChain();

	surface.getWindow().get()._event_size_list.emplace_back([&](const Window&, const Rect& rect) {

		needsResize = currentSize.components != rect.size.components;
		currentSize = rect.size;
	});
}

Borrow<VulkanGraphicsSwapChain::VulkanFrame>
	VulkanGraphicsSwapChain::getFrame() {

	if (needsResize) {
		device.waitIdle();
		createSwapChain();

		needsResize = false;
	}

	try {
		const auto result = device.acquireNextImageKHR(swapChain.get(), 32'000'000, swapChainSemaphore.get(), nullptr);

		return Borrow { activeFrames[result.value] };
	} catch(const vk::OutOfDateKHRError&) {
		return Borrow(activeFrames[0]);
	}
}

void
	VulkanGraphicsSwapChain::createSwapChain() {

	auto& dev_surface = this->surface.get();

	auto capabilities = physicalDevice.getSurfaceCapabilitiesKHR(dev_surface);
	auto formats = physicalDevice.getSurfaceFormatsKHR(dev_surface);
	auto presents = physicalDevice.getSurfacePresentModesKHR(dev_surface);

	vk::SwapchainCreateInfoKHR createSwapChain;

	createSwapChain.surface = dev_surface;

	createSwapChain.imageArrayLayers = 1;
	createSwapChain.imageColorSpace = formats[0].colorSpace;
	createSwapChain.imageExtent = getExtent(currentSize, capabilities);
	createSwapChain.imageFormat = formats[0].format;
	createSwapChain.imageUsage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eColorAttachment;

	createSwapChain.presentMode = presents[0];

	createSwapChain.minImageCount = std::min(capabilities.minImageCount + 1, capabilities.maxImageCount);

	assert(presentQueue.supportsPresent(*this));

	if (presentQueue.familyIndex != graphicsQueue.familyIndex) {

		createSwapChain.imageSharingMode = vk::SharingMode::eConcurrent;

		createSwapChain.queueFamilyIndexCount = 2;
		createSwapChain.pQueueFamilyIndices = std::array<uint32_t, 2> { presentQueue.familyIndex, graphicsQueue.familyIndex }.data();
	}
	else {
		createSwapChain.imageSharingMode = vk::SharingMode::eExclusive;
	}

	if (swapChain) {

		const auto oldSwapChain = swapChain.release();
		createSwapChain.oldSwapchain = oldSwapChain;

		swapChain = device.createSwapchainKHRUnique(createSwapChain);

		for (const auto& item : activeFrames) {

			auto& frame = static_cast<VulkanFrame&>(item.get());

			frame.image = vk::Image();
			device.destroyImageView(frame.imageView.release());
		}

		activeFrames.clear();

		device.destroySwapchainKHR(oldSwapChain);
	}
	else {

		swapChain = device.createSwapchainKHRUnique(createSwapChain);
	}

	auto images = device.getSwapchainImagesKHR(swapChain.get());
	activeFrames.reserve(images.size());

	uint32_t index{0};

	for (const vk::Image& image : images) {

		vk::ImageViewCreateInfo createImageView;

		createImageView.image = image;
		createImageView.format = formats[0].format;

		createImageView.viewType = vk::ImageViewType::e2D;

		createImageView.components = vk::ComponentMapping();
		createImageView.subresourceRange = vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);

		auto frame = new VulkanFrame();

		frame->index = index;
		frame->image = image;
		frame->imageView = device.createImageViewUnique(createImageView);

		activeFrames.emplace_back(frame);

		index += 1;
	}
}


vk::Extent2D
	getExtent(const Size2D &size, const vk::SurfaceCapabilitiesKHR &capabilities) {

	if (capabilities.currentExtent.width == UINT_MAX) {

		vk::Extent2D extent;

		extent.width =
			std::min(
				std::max(
					uint32_t(size.x()),
					capabilities.minImageExtent.width),
				capabilities.maxImageExtent.width);

		extent.height =
			std::min(
				std::max(
					uint32_t(size.y()),
					capabilities.minImageExtent.height),
				capabilities.maxImageExtent.height);

		return extent;
	} else {
		return capabilities.currentExtent;
	}
}
