#pragma once

#include <headerconv.hpp>

#include <vulkanGraphicsContext.hpp>
#include <vulkanGraphicsSwapChain.hpp>
#include <vulkanGraphicsQueue.hpp>

#include <windowSurface.hpp>

#include "ownership.hpp"

SNOW_OWL_NAMESPACE(app)

struct Renderer {

	gx::VulkanGraphicsContext             context;

	cx::Own<gx::VulkanGraphicsQueue>      graphicsQueue;

	cx::Own<gx::VulkanGraphicsSwapChain>  swapChain;


	vk::UniqueCommandPool                 commandPool;

	vk::UniqueSemaphore                   presentReadySemaphore;


	std::vector<vk::UniqueCommandBuffer>  clearBuffer;

	Renderer(const ui::WindowSurface &surface);

	void frame();
};

SNOW_OWL_NAMESPACE_END
