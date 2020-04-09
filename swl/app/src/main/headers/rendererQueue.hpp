#pragma once

#include <headerconv.hpp>
#include <vulkanGraphicsContext.hpp>

SNOW_OWL_NAMESPACE(app)

struct RendererQueue {

	vk::Queue queue;

	RendererQueue(const vk::Queue &queue);
};

SNOW_OWL_NAMESPACE_END
