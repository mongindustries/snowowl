//
// Created by Michael Ong on 12/4/20.
//
#include "vulkanGraphicsSwapChain.hpp"

#include "vulkanGraphicsContext.hpp"
#include "vulkanGraphicsBackend.hpp"

using namespace swl::cx;
using namespace swl::gx;

vk::Extent2D getExtent(const swl::ui::WindowSurface &surface, const vk::SurfaceCapabilitiesKHR &capabilities);

VulkanGraphicsSwapChain::VulkanGraphicsSwapChain(
	const VulkanGraphicsContext &context,
	const VulkanGraphicsQueue& graphicsQueue,
	const VulkanGraphicsQueue& presentQueue,
	const swl::ui::WindowSurface &surface) : GraphicsSwapChain(context, surface),

	device  (context._device.get()),
	surface (backend::VulkanGraphicsBackend::instance->makeSurface(context._instance.get(), surface)){

	vk::SemaphoreCreateInfo createSemaphore;
	swapChainSemaphore = device.createSemaphoreUnique(createSemaphore);

	auto &dev_surface = this->surface.get();

	auto capabilities = context._active_device.getSurfaceCapabilitiesKHR(dev_surface);
	auto formats      = context._active_device.getSurfaceFormatsKHR     (dev_surface);
	auto presents     = context._active_device.getSurfacePresentModesKHR(dev_surface);

	vk::SwapchainCreateInfoKHR createSwapChain;

	createSwapChain.surface           = dev_surface;

	createSwapChain.imageArrayLayers  = 1;
	createSwapChain.imageColorSpace   = formats[0].colorSpace;
	createSwapChain.imageExtent       = getExtent(surface, capabilities);
	createSwapChain.imageFormat       = formats[0].format;
	createSwapChain.imageUsage        = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eColorAttachment;

	createSwapChain.presentMode       = presents[0];

	createSwapChain.minImageCount     = std::min(capabilities.minImageCount + 1, capabilities.maxImageCount);

	assert(presentQueue.supportsPresent(*this));

	if (presentQueue.familyIndex != graphicsQueue.familyIndex) {

		createSwapChain.imageSharingMode      = vk::SharingMode::eConcurrent;

		createSwapChain.queueFamilyIndexCount = 2;
		createSwapChain.pQueueFamilyIndices   = std::array<uint32_t, 2> { presentQueue.familyIndex, graphicsQueue.familyIndex }.data();
	} else {
		createSwapChain.imageSharingMode      = vk::SharingMode::eExclusive;
	}

	swapChain = device.createSwapchainKHRUnique(createSwapChain);

	auto images = device.getSwapchainImagesKHR(swapChain.get());

	for (const vk::Image &image : images) {

		vk::ImageViewCreateInfo createImageView;

		createImageView.image   = image;
		createImageView.format  = formats[0].format;

		createImageView.viewType          = vk::ImageViewType::e2D;

		createImageView.components        = vk::ComponentMapping();
		createImageView.subresourceRange  = vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);

		auto* frame = new VulkanFrame();

		frame->image     = image;
		frame->imageView = device.createImageViewUnique(createImageView);

		activeFrames.emplace_back(frame);
	}
}

Borrow<VulkanGraphicsSwapChain::Frame>
	VulkanGraphicsSwapChain::getFrame() const {

	uint32_t index;
	device.acquireNextImageKHR(swapChain.get(), UINT64_MAX, swapChainSemaphore.get(), nullptr, &index);

	return Borrow(activeFrames[index]);
}


vk::Extent2D
	getExtent(const swl::ui::WindowSurface &surface, const vk::SurfaceCapabilitiesKHR &capabilities) {

	if (capabilities.currentExtent.width == UINT_MAX) {

		auto size = surface.getSize();

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
