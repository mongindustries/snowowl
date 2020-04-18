#define _USE_MATH_DEFINES

#include <vector>
#include <cmath>

#include <vulkan_fence_lock.hpp>
#include <vulkan_bufer_record.hpp>

#include <file_manager.hpp>

#include <event.hpp>

#include "renderer.hpp"

namespace SWL {
		using namespace swl::cx;
		using namespace swl::ui;
		using namespace swl::gx;
}

using namespace std;

using namespace SWL;
using namespace swl::app;

Renderer::Renderer(const WindowSurface &surface):
	graphicsQueue (new VulkanGraphicsQueue(context, vk::QueueFlagBits::eGraphics)),
	swapChain     (nullptr),
	shader_vert   (nullptr),
	shader_frag   (nullptr) {

	context.createDevice({ MutableBorrow(graphicsQueue) });

	auto& g_queue = graphicsQueue.get();

	swapChain     = new VulkanGraphicsSwapChain(context, g_queue, g_queue, surface);
	commandPool   = g_queue.commandPool();

	clearBuffer = context._device->allocateCommandBuffersUnique({
		commandPool.get(), vk::CommandBufferLevel::ePrimary, (uint32_t) swapChain->active_frames.size() });

	presentReadySemaphore = context._device->createSemaphoreUnique({ });
	fence = context._device->createFenceUnique({ });

	shader_vert = new VulkanShader(context, cx::FileManager::resourcePath / L"simple-vert.spv");
	shader_frag = new VulkanShader(context, cx::FileManager::resourcePath / L"simple-frag.spv");

	auto& swap_chain = swapChain.get();

	swap_chain.recreate_size_events.push_back(Event{ std::function<void()>{ [&]() { create_framebuffers(); } } });

	create_framebuffers();
}

void Renderer::create_framebuffers() {

	// defines UAV that this render pass is going to use (on any sub-pass)
	array attachmentDescription{
		vk::AttachmentDescription{ {},
			 swapChain->format, vk::SampleCountFlagBits::e1,
			 vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore,
			 vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare,
			 vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR }
	};

	// defines a sub-pass attachment, attachment index is referred from attachmentDescription.
	array attachmentReferences{
		vk::AttachmentReference {0, vk::ImageLayout::eColorAttachmentOptimal }
	};
	// a list of sub-passes for a render pass
	array subpasses{
		vk::SubpassDescription{ {},
			vk::PipelineBindPoint::eGraphics,
			0, nullptr,
			attachmentReferences.size(), attachmentReferences.data() }
	};

	array dependencies{
		vk::SubpassDependency{
			VK_SUBPASS_EXTERNAL,
			{},

			vk::PipelineStageFlagBits::eColorAttachmentOutput,
			vk::PipelineStageFlagBits::eColorAttachmentOutput,

			{},
			vk::AccessFlagBits::eColorAttachmentWrite,

			vk::DependencyFlagBits::eByRegion
		},
		vk::SubpassDependency{
			{},
			VK_SUBPASS_EXTERNAL,

			vk::PipelineStageFlagBits::eColorAttachmentOutput,
			vk::PipelineStageFlagBits::eBottomOfPipe,

			vk::AccessFlagBits::eColorAttachmentWrite,
			{},

			vk::DependencyFlagBits::eByRegion
		}
	};

	renderPass = context._device->createRenderPassUnique({ {},
		attachmentDescription.size(), attachmentDescription.data(),
		subpasses.size(), subpasses.data(),
		dependencies.size(), dependencies.data()
	});

	framebuffers.resize(swapChain->active_frames.size());

	uint8_t index{ 0 };
	for (const auto& frame : swapChain->active_frames) {

		auto& imageview = frame.get().image_view.get();
		auto size = swapChain->current_size;

		vk::FramebufferCreateInfo framebuffer{ {}, renderPass.get(), 1, &imageview, uint32_t(size.x()), uint32_t(size.y()), 1 };
		framebuffers[index++] = context._device->createFramebufferUnique(framebuffer);
	}
}

float __color__ = 0;
float ceillllll = std::sin(M_PI_2);

float xeillllll = std::cos(M_2_PI);

void Renderer::frame() {

	auto frame = swapChain->getFrame();

	{ VulkanFenceLock lock(context._device.get(), fence.get());

		auto& image   = frame.get().image;
		auto& buffer  = clearBuffer[frame.get().index].get();

		{ VulkanBufferRecord record { buffer, vk::CommandBufferUsageFlagBits::eOneTimeSubmit };

			float constcol = std::sin(__color__) / ceillllll;
			float xonstcol = std::cos(__color__) / xeillllll;

			auto color     = vk::ClearColorValue(array{ constcol, 1.0f - constcol, xonstcol, 1.0f });

			__color__ += 0.01;

			vk::RenderPassBeginInfo beginInfo{ renderPass.get(), framebuffers[frame.get().index].get()};

			beginInfo.renderArea      = vk::Rect2D{
				{},
				{ (uint32_t) swapChain->current_size.x(), (uint32_t) swapChain->current_size.y() }};
			beginInfo.clearValueCount = 1;
			beginInfo.pClearValues    = array{ vk::ClearValue(color) }.data();

			record->beginRenderPass (beginInfo, vk::SubpassContents::eInline);

			record->endRenderPass   ();
		}

		const auto rp = vector{ presentReadySemaphore.get() };
		graphicsQueue->submit({ buffer }, VulkanGraphicsQueue::GPUWaitType::semaphores({ }, rp), lock.fence);
	}

	const auto sc = vector{ frame };
	const auto sp = vector{ presentReadySemaphore.get(), swapChain->semaphore.get() };

	graphicsQueue->present(sc, VulkanGraphicsQueue::GPUWaitType::semaphores(sp, { }));
}
