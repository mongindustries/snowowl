#pragma once

#include <headerconv.hpp>
#include <vulkanGraphicsContext.hpp>

SNOW_OWL_NAMESPACE(app)
	struct RendererBuffer;

	struct RendererQueue {

	unsigned int           index;

	vk::Device const*      device{};
	vk::Queue              queue;

	vk::UniqueCommandPool  command_pool;

	RendererQueue();
	
	RendererQueue(unsigned int index, const vk::Device &device);

	
	void submit(const RendererBuffer &buffers) const;
};

SNOW_OWL_NAMESPACE_END
