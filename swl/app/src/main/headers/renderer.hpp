#pragma once

#include <headerconv.hpp>
#include <vulkanGraphicsContext.hpp>

#include <window.hpp>
#include <windowSurface.hpp>

#include "rendererQueue.hpp"

SNOW_OWL_NAMESPACE(app)

struct Renderer {

	vk::Instance&       _instance;
	vk::PhysicalDevice& _device;

	vk::Device          _logicalDevice;

	ui::WindowSurface   _surface;


	RendererQueue _graphicsQueue;
	RendererQueue _presentQueue;

	Renderer(const gx::implem::VulkanGraphicsContext& context, const ui::Window& target_window);
};

SNOW_OWL_NAMESPACE_END
