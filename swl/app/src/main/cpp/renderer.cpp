#include <vector>
#include <iostream>

#include "renderer.hpp"

namespace SWL {
	using namespace swl::cx;
	using namespace swl::ui;
	using namespace swl::gx;
}

using namespace std;

using namespace SWL;
using namespace swl::app;

Renderer::Renderer(const ui::WindowSurface &surface):
	context       (),
	graphicsQueue (new VulkanGraphicsQueue(context, vk::QueueFlagBits::eGraphics)),
	swapChain     (nullptr) {

	context.createDevice({ MutableBorrow(graphicsQueue) });

	auto& g_queue = graphicsQueue.get();

	swapChain     = new VulkanGraphicsSwapChain(context, g_queue, g_queue, surface);
	commandPool   = g_queue.commandPool();

	vk::CommandBufferAllocateInfo allocate(commandPool.get(), vk::CommandBufferLevel::ePrimary, swapChain->activeFrames.size());
	clearBuffer = context._device->allocateCommandBuffersUnique(allocate);

	vk::SemaphoreCreateInfo sema_create{};
	presentReadySemaphore = context._device->createSemaphoreUnique(sema_create);
}

float __color__ = 0;

void Renderer::frame() {

	auto frame  = swapChain->getFrame();
	auto& image = frame.get().image;

	auto& buffer = clearBuffer[frame.get().index].get();

	const vk::CommandBufferBeginInfo recordBegin{ { vk::CommandBufferUsageFlagBits::eOneTimeSubmit } };
	buffer.begin(recordBegin);

	auto subrange = vk::ImageSubresourceRange{ vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 };
	auto color    = vk::ClearColorValue(array {__color__, 0.25f, 0.55f, 1.0f });

	__color__ += 0.01;

	if (__color__ > 1) {
		__color__ = 0;
	}

	vk::ImageMemoryBarrier barrier{};

	barrier.srcAccessMask = vk::AccessFlagBits::eMemoryRead;
	barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

	barrier.oldLayout = vk::ImageLayout::eUndefined;
	barrier.newLayout = vk::ImageLayout::eTransferDstOptimal;

	barrier.srcQueueFamilyIndex = graphicsQueue.get().familyIndex;
	barrier.dstQueueFamilyIndex = graphicsQueue.get().familyIndex;

	barrier.image = image;
	barrier.subresourceRange = subrange;

	array from { barrier };
	buffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eTransfer, { }, { }, { }, from);

	buffer.clearColorImage(image, vk::ImageLayout::eTransferDstOptimal, &color, 1, &subrange);

	barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
	barrier.dstAccessMask = vk::AccessFlagBits::eMemoryRead;

	barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
	barrier.newLayout = vk::ImageLayout::ePresentSrcKHR;

	array to { barrier };
	buffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eBottomOfPipe, { }, { }, { }, to);

	buffer.end();

	const auto sp = vector { swapChain->swapChainSemaphore.get() };
	const auto rp = vector { presentReadySemaphore.get() };

	graphicsQueue->submit({ buffer }, VulkanGraphicsQueue::WaitType::idle());

	const auto sc = vector { pair { Borrow(swapChain), frame } };
	graphicsQueue->present(sc, VulkanGraphicsQueue::WaitType::semaphores(sp, { }));
}