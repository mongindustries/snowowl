#include "rendererSwapchain.hpp"
#include "renderer.hpp"

using namespace std;
using namespace swl::app;

RendererSwapchain::RendererSwapchain(
	const Renderer* renderer,
	const ui::Window& window): renderer(renderer) {

	createSwapchain(window, nullptr);
	
	const std::function<void(const ui::Window&, const cx::Rect&)> resizeCallback = 
		std::bind(&RendererSwapchain::resize, this, std::placeholders::_1, std::placeholders::_2);
	
	const_cast<ui::Window&>(window)._event_size_list.emplace_back(resizeCallback);
}

auto RendererSwapchain::getNextImage(const vk::Semaphore& signal) const -> std::tuple<uint32_t, vk::Image, vk::ImageView> {

	uint32_t nextImage;
	renderer->logical_device->acquireNextImageKHR(swapchain.get(), UINT64_MAX, signal, nullptr, &nextImage);

	auto &_pair = swapchain_views[nextImage];
	return tuple{ nextImage, get<0>(_pair), get<1>(_pair).get() };
}

void RendererSwapchain::present(const vk::Queue& queue, uint32_t swapchainFrameIndex, const vk::Semaphore& wait) const {

	const vk::PresentInfoKHR presentInfo(1, &wait, 1, &swapchain.get(), &swapchainFrameIndex);
	assert(queue.presentKHR(presentInfo) == vk::Result::eSuccess);
}

void RendererSwapchain::resize(const ui::Window& window, const cx::Rect& new_rect) {

	renderer->logical_device->waitIdle();
	
	auto oldSwapchain = swapchain.release();
	
	createSwapchain(window, &oldSwapchain);
	renderer->logical_device->destroySwapchainKHR(oldSwapchain);
}

void RendererSwapchain::createSwapchain(const ui::Window& window, vk::SwapchainKHR* oldSwapChain) {

	if (!swapchain_views.empty()) {
		swapchain_views.clear();
	}
	
	auto& logical_device = renderer->logical_device;
	auto& surface = renderer->surface;

	auto surface_formats = renderer->physical_device.getSurfaceFormatsKHR(renderer->surface);
	const auto surface_capabilities = renderer->physical_device.getSurfaceCapabilitiesKHR(renderer->surface);

	auto presentModes = renderer->physical_device.getSurfacePresentModesKHR(renderer->surface);

	vk::Extent2D swapchainExtent;
	if (surface_capabilities.currentExtent.width == UINT_MAX) {
		swapchainExtent.width =
			min(
				max(
					uint32_t(window.getSize().x()),
					surface_capabilities.minImageExtent.width),
				surface_capabilities.maxImageExtent.width);
		swapchainExtent.height =
			min(
				max(
					uint32_t(window.getSize().y()),
					surface_capabilities.minImageExtent.height),
				surface_capabilities.maxImageExtent.height);
	}
	else {
		swapchainExtent = surface_capabilities.currentExtent;
	}

	const auto format =
		(surface_formats[0] != vk::Format::eUndefined) ? surface_formats[0].format : vk::Format::eB8G8R8A8Unorm;

	vk::SwapchainCreateInfoKHR createSwapchainInfo({},
		surface,
		surface_capabilities.minImageCount + 1,
		format,
		vk::ColorSpaceKHR::eSrgbNonlinear,
		swapchainExtent,
		1,
		vk::ImageUsageFlagBits::eColorAttachment,
		vk::SharingMode::eExclusive);

	createSwapchainInfo.presentMode = presentModes[0];
	createSwapchainInfo.oldSwapchain = oldSwapChain != nullptr ? *oldSwapChain : nullptr;

	auto &pqueue = *renderer->present_queue;
	auto &gqueue = *renderer->graphics_queue;

	if (pqueue.index != gqueue.index) {
		createSwapchainInfo.imageSharingMode = vk::SharingMode::eConcurrent;

		unsigned int queues[2] = {
			pqueue.index,
			gqueue.index
		};
		createSwapchainInfo.queueFamilyIndexCount = 2;
		createSwapchainInfo.pQueueFamilyIndices = queues;
	}

	swapchain = logical_device->createSwapchainKHRUnique(createSwapchainInfo);

	auto images = logical_device->getSwapchainImagesKHR(swapchain.get());

	for (const auto &image : images) {

		vk::ImageViewCreateInfo createImageView({}, image, vk::ImageViewType::e2D);

		createImageView.format = format;
		
		createImageView.components.r = vk::ComponentSwizzle::eIdentity;
		createImageView.components.g = vk::ComponentSwizzle::eIdentity;
		createImageView.components.b = vk::ComponentSwizzle::eIdentity;
		createImageView.components.a = vk::ComponentSwizzle::eIdentity;
		
		createImageView.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		createImageView.subresourceRange.baseMipLevel = 0;
		createImageView.subresourceRange.levelCount = 1;
		createImageView.subresourceRange.baseArrayLayer = 0;
		createImageView.subresourceRange.layerCount = 1;

		swapchain_views.emplace_back(image, logical_device->createImageViewUnique(createImageView));
	}
}
