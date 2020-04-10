#include "rendererBufferBasicClear.hpp"

using namespace swl::app;

RendererBufferBasicClear::RendererBufferBasicClear(const RendererQueue& queue) : RendererBuffer(queue) {
	
}

void RendererBufferBasicClear::record(unsigned int index, const vk::CommandBuffer& buffer) {

	if (imageToClear != nullptr) {

		vk::ClearColorValue clearColor(std::array<float, 4> {
			22.0f / 255.0f,
			81 / 255.0f,
			100 / 255.0f,
			255 / 255.0f });

		vk::ImageSubresourceRange range{};

		range.aspectMask = vk::ImageAspectFlagBits::eColor;
		range.levelCount = 1;
		range.layerCount = 1;
		
		buffer.clearColorImage(*imageToClear, vk::ImageLayout::eGeneral, &clearColor, 1, &range);
	}
}
