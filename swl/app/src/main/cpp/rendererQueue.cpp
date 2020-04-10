#include "rendererQueue.hpp"
#include "rendererBuffer.hpp"

#include <algorithm>
#include <vector>

using namespace std;
using namespace swl::app;

RendererQueue::RendererQueue(): index(-1) { }

RendererQueue::RendererQueue(unsigned index, const vk::Device& device):
	index  (index),
	device (&device),
	queue  (device.getQueue(index, 0)) {

	const vk::CommandPoolCreateInfo createCommandPool({}, index);
	command_pool = device.createCommandPoolUnique(createCommandPool);
}

void RendererQueue::submit(const RendererBuffer& buffers) const {

	vk::SubmitInfo submitInfo;

	vector<vk::CommandBuffer> _buffers(buffers.buffers.size());
	transform(buffers.buffers.begin(), buffers.buffers.end(), _buffers.begin(), [](const auto& item) { return item.get(); });

	submitInfo.commandBufferCount = _buffers.size();
	submitInfo.pCommandBuffers = _buffers.data();

	assert(queue.submit(1, &submitInfo, nullptr) == vk::Result::eSuccess);
}
