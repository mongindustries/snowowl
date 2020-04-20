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


	vk::UniqueCommandPool                 commandPool;

	vk::UniqueSemaphore                   presentReadySemaphore;

	vk::UniqueFence                       fence;


	vk::UniquePipeline                    graphicsPipeline;


	vk::UniqueRenderPass                  renderPass;

	vk::UniqueDescriptorSetLayout         descriptorSetLayout;

	vk::UniquePipelineLayout              pipelineLayout;


	std::vector<vk::UniqueCommandBuffer>  clearBuffer;

	std::vector<vk::UniqueFramebuffer>    framebuffers;

	Renderer(const ui::WindowSurface &surface);


	void create_uniform_buffers   ();

	void create_render_pass       ();

	void create_framebuffers      ();

	void create_graphics_pipeline ();

	void create_pipeline_bindings ();


	void update();
	
	void frame();
};

SNOW_OWL_NAMESPACE_END
