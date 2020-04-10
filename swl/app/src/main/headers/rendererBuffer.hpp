#pragma once

#include <vector>

#include "headerconv.hpp"

#include <vulkanGraphicsContext.hpp>

SNOW_OWL_NAMESPACE(app)

struct RendererQueue;

struct RendererBuffer {

	explicit RendererBuffer(const RendererQueue& queue);

	virtual ~RendererBuffer() = default;


	void execute();
	

	virtual void record(unsigned int index, const vk::CommandBuffer &buffer) = 0;

	friend RendererQueue;

private:

	std::vector<vk::UniqueCommandBuffer> buffers;

	RendererQueue const& queue;
};

SNOW_OWL_NAMESPACE_END
