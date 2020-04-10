#pragma once

#include <headerconv.hpp>
#include <vulkanGraphicsContext.hpp>

#include <windowSurface.hpp>

#include "ownership.hpp"
#include "rendererBuffer.hpp"
#include "rendererQueue.hpp"
#include "rendererSwapchain.hpp"

SNOW_OWL_NAMESPACE(app)

struct Renderer {

	vk::Instance&       instance;
	vk::PhysicalDevice  physical_device;

	vk::UniqueDevice    logical_device;

	vk::SurfaceKHR      surface;

	vk::UniqueSemaphore frameSemaphore;


	cx::Own<RendererQueue>       graphics_queue;
	cx::Own<RendererQueue>       present_queue;

	cx::Own<RendererSwapchain>   swapchain;


	cx::Own<RendererBuffer>      commands;

	Renderer(const gx::implem::VulkanGraphicsContext& context, const ui::Window& target_window);


	void frame();
};

SNOW_OWL_NAMESPACE_END
