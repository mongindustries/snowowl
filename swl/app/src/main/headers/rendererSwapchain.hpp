#pragma once

#include <headerconv.hpp>

#include <vector>

#include <window.hpp>
#include <vulkanGraphicsContext.hpp>

SNOW_OWL_NAMESPACE(app)

struct Renderer;

struct RendererSwapchain {

	vk::UniqueSwapchainKHR
		swapchain;

	Renderer const*
		renderer{};

	std::vector<std::pair<vk::Image, vk::UniqueImageView>>
		swapchain_views;
	

	RendererSwapchain() = default;

	explicit RendererSwapchain(const Renderer* renderer, const ui::Window& window);


	auto getNextImage(const vk::Semaphore& signal) const -> std::tuple<uint32_t, vk::Image, vk::ImageView>;

	void present(const vk::Queue& queue, uint32_t swapchainFrameIndex, const vk::Semaphore& wait) const;
	
private:

	void createSwapchain(const ui::Window& window, vk::SwapchainKHR* oldSwapChain);
	
	void resize(const ui::Window&, const cx::Rect& new_rect);
};

SNOW_OWL_NAMESPACE_END
