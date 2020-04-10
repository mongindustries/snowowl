#pragma once

#include "rendererBuffer.hpp"
#include "rendererSwapchain.hpp"

SNOW_OWL_NAMESPACE(app)

struct RendererBufferBasicClear: public RendererBuffer {

	RendererBufferBasicClear(const RendererQueue& queue);

	void record(unsigned int index, const vk::CommandBuffer& buffer) override;


	vk::Image* imageToClear{};
};

SNOW_OWL_NAMESPACE_END
