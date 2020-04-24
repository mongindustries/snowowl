//
// Created by Michael Ong on 12/4/20.
//
#include <algorithm>

#include <window.hpp>

#include "vulkan_context.hpp"
#include "vulkan_graphics_backend.hpp"

#include "vulkan_swap_chain.hpp"
#include "vulkan_queue.hpp"

#undef min
#undef max

using namespace swl::cx;
using namespace swl::gx;
using namespace swl::ui;

vk::Extent2D getExtent(const size_2d &size, const vk::SurfaceCapabilitiesKHR &capabilities);

VulkanGraphicsSwapChain::VulkanGraphicsSwapChain(
	const VulkanGraphicsContext &context,
	const VulkanGraphicsQueue& graphicsQueue,
	const VulkanGraphicsQueue& presentQueue,
	const swl::ui::WindowSurface &surface) : GraphicsSwapChain(context, surface),

	physical_device(context._active_device),
	device         (context._device.get()),

	surface        (backend::VulkanGraphicsBackend::instance->makeSurface(context._instance.get(), surface)),

	queue_graphics (presentQueue),
	queue_present  (graphicsQueue),

	current_size   (surface.getSize()) {

	semaphore = device.createSemaphoreUnique({});

	createSwapChain();

	surface.getWindow().get()._event_size_list.emplace_back([&](const Window&, const rect& rect) {

		needs_resize = current_size.components != rect.size.components;
		current_size = rect.size;
	});
}

exp::ptr_ref<VulkanFrame>
	VulkanGraphicsSwapChain::getFrame() {

	if (needs_resize) {

		device.waitIdle();
		createSwapChain();

		needs_resize = false;
	}

	try {
		const auto result = device.acquireNextImageKHR(swap_chain.get(), 32'000'000, semaphore.get(), nullptr);

		return exp::ptr_ref{active_frames[result.value] };
	} catch(const vk::OutOfDateKHRError&) {
		return exp::ptr_ref{active_frames[0] };
	}
}

void
	VulkanGraphicsSwapChain::createSwapChain() {

	auto& dev_surface = this->surface.get();

	auto capabilities = physical_device.getSurfaceCapabilitiesKHR(dev_surface);
	auto formats = physical_device.getSurfaceFormatsKHR(dev_surface);
	auto presents = physical_device.getSurfacePresentModesKHR(dev_surface);

	vk::SwapchainCreateInfoKHR createSwapChain;

	createSwapChain.surface = dev_surface;

	createSwapChain.imageArrayLayers = 1;
	createSwapChain.imageColorSpace = formats[0].colorSpace;
	createSwapChain.imageExtent = getExtent(current_size, capabilities);
	createSwapChain.imageFormat = formats[0].format;
	createSwapChain.imageUsage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eColorAttachment;

	createSwapChain.presentMode = presents[0];

	createSwapChain.minImageCount = std::min(capabilities.minImageCount + 1, capabilities.maxImageCount);

	assert(queue_present.present_support(*this));

	if (queue_graphics.family_index != queue_present.family_index) {

		createSwapChain.imageSharingMode = vk::SharingMode::eConcurrent;

		std::array queues{queue_present.family_index, queue_graphics.family_index };
		createSwapChain.queueFamilyIndexCount = queues.size();
		createSwapChain.pQueueFamilyIndices   = queues.data();
	}
	else {
		createSwapChain.imageSharingMode = vk::SharingMode::eExclusive;
	}

	if (swap_chain) {

		const auto oldSwapChain = swap_chain.release();
		createSwapChain.oldSwapchain = oldSwapChain;

		swap_chain = device.createSwapchainKHRUnique(createSwapChain);

		for (const auto& item : active_frames) {

			auto& frame = static_cast<VulkanFrame&>(item.get());

			frame.image = vk::Image();
			device.destroyImageView(frame.image_view.release());
		}

		active_frames.clear();

		device.destroySwapchainKHR(oldSwapChain);
	}
	else {

		swap_chain = device.createSwapchainKHRUnique(createSwapChain);
	}

	format = formats[0].format;

	auto images = device.getSwapchainImagesKHR(swap_chain.get());
	active_frames.reserve(images.size());

	uint32_t index{0};

	for (const vk::Image& image : images) {

		vk::ImageViewCreateInfo createImageView;

		createImageView.image = image;
		createImageView.format = formats[0].format;

		createImageView.viewType = vk::ImageViewType::e2D;

		createImageView.components = vk::ComponentMapping();
		createImageView.subresourceRange = vk::ImageSubresourceRange {
			vk::ImageAspectFlagBits::eColor,
			0, 1,
			0, 1 };

		active_frames.emplace_back(new VulkanFrame {
			.index        = index,
			.image        = image,
			.image_view   = device.createImageViewUnique(createImageView),
			.format       = formats[0].format,
			.swap_chain   = *this
		});

		index += 1;
	}

	for (const auto& event : recreate_size_events) {
		event.invoke();
	}
}


vk::Extent2D
	getExtent(const size_2d &size, const vk::SurfaceCapabilitiesKHR &capabilities) {

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
