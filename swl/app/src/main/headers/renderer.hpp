#pragma once

#include <headerconv.hpp>
#include <vulkanGraphicsContext.hpp>

#include <window.hpp>
#include <windowSurface.hpp>

SNOW_OWL_NAMESPACE(app)

struct Renderer {

	vk::Instance&     _instance;
	vk::Device&       _device;

	ui::WindowSurface _surface;

	Renderer(const gx::implem::VulkanGraphicsContext& context, const ui::Window& target_window);
};

SNOW_OWL_NAMESPACE_END
