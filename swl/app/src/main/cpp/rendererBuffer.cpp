#include "rendererBuffer.hpp"
#include "rendererQueue.hpp"


swl::app::RendererBuffer::RendererBuffer(const RendererQueue& queue): queue(queue) {

	const vk::CommandBufferAllocateInfo allocate(
		queue.command_pool.get(),
		vk::CommandBufferLevel::ePrimary,
		1);

	buffers = queue.device->allocateCommandBuffersUnique(allocate);
}

void swl::app::RendererBuffer::execute() {

	uint32_t index = 0;
	std::vector<vk::SubmitInfo> submits;
	
	for (const auto &buffer : buffers) {

		vk::CommandBufferBeginInfo cmdBufferBegin(vk::CommandBufferUsageFlagBits::eSimultaneousUse);

		auto& rbuffer = buffer.get();
		rbuffer.begin(cmdBufferBegin);

		record(index += 1, rbuffer);

		rbuffer.end();
	}
}
