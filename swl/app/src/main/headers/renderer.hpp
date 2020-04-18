#pragma once

#include <header.hpp>

#include <vulkan_context.hpp>
#include <vulkan_shader.hpp>
#include <vulkan_swap_chain.hpp>
#include <vulkan_queue.hpp>

#include <windowSurface.hpp>

#include "ownership.hpp"

SNOW_OWL_NAMESPACE(app)

struct Renderer {

	gx::VulkanGraphicsContext             context;

	cx::Own<gx::VulkanGraphicsQueue>      graphicsQueue;

	cx::Own<gx::VulkanGraphicsSwapChain>  swapChain;

	cx::Own<gx::VulkanShader>             shader_vert;
	cx::Own<gx::VulkanShader>             shader_frag;


	vk::UniqueCommandPool                 commandPool;

	vk::UniqueSemaphore                   presentReadySemaphore;

	vk::UniqueFence                       fence;

	vk::UniqueRenderPass                  renderPass;


	std::vector<vk::UniqueCommandBuffer>  clearBuffer;

	std::vector<vk::UniqueFramebuffer>    framebuffers;

	Renderer(const ui::WindowSurface &surface);

	void create_framebuffers();

	
	void frame();
};

SNOW_OWL_NAMESPACE_END
