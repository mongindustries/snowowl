#define _USE_MATH_DEFINES

#include <vector>
#include <cmath>

#define CPP_SHADER
#include "../hlsl/common.h"

#include <vulkan_fence_lock.hpp>
#include <vulkan_buffer_recorder.hpp>

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
	swapChain     (nullptr) {

	context.createDevice({ MutableBorrow(graphicsQueue) });

	auto& g_queue = graphicsQueue.get();

	swapChain     = new VulkanGraphicsSwapChain(context, g_queue, g_queue, surface);
	commandPool   = g_queue.commandPool();

	clearBuffer = context._device->allocateCommandBuffersUnique({
		commandPool.get(), vk::CommandBufferLevel::ePrimary, (uint32_t) swapChain->active_frames.size() });

	presentReadySemaphore = context._device->createSemaphoreUnique({ });
	fence = context._device->createFenceUnique({ });

	auto& swap_chain = swapChain.get();

	swap_chain.recreate_size_events.push_back(Event{ std::function<void()>{ [&]() { create_framebuffers(); } } });

	create_render_pass();
	create_graphics_pipeline();

	create_framebuffers();
}

void Renderer::create_uniform_buffers   () {


}

void Renderer::create_render_pass       () {

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
}

void Renderer::create_framebuffers      () {

	framebuffers.resize(swapChain->active_frames.size());

	uint8_t index{ 0 };
	for (const auto& frame : swapChain->active_frames) {

		auto& imageview = frame.get().image_view.get();
		auto size = swapChain->current_size;

		vk::FramebufferCreateInfo framebuffer{ {},
			 renderPass.get(),
			 1,
			 &imageview,
			 uint32_t(size.x()),
			 uint32_t(size.y()),
			 1 };
		framebuffers[index++] = context._device->createFramebufferUnique(framebuffer);
	}
}

void Renderer::create_graphics_pipeline () {

	array bindings{
		vk::DescriptorSetLayoutBinding{
			0,
			vk::DescriptorType::eUniformBuffer,
			1, vk::ShaderStageFlagBits::eVertex,
			nullptr } };

	descriptorSetLayout = context._device->createDescriptorSetLayoutUnique({
		{}, bindings.size(), bindings.data() });

	array setLayouts{
		descriptorSetLayout.get()
	};

	pipelineLayout = context._device->createPipelineLayoutUnique({
		{}, setLayouts.size(), setLayouts.data(), 0, nullptr });

	auto shader_vert = VulkanShader(context, cx::FileManager::resourcePath / L"simple-vert.spv");
	auto shader_frag = VulkanShader(context, cx::FileManager::resourcePath / L"simple-frag.spv");

	array shaders{
		vk::PipelineShaderStageCreateInfo{ {}, vk::ShaderStageFlagBits::eVertex,    shader_vert.shader.get(), "main" },
		vk::PipelineShaderStageCreateInfo{ {}, vk::ShaderStageFlagBits::eFragment,  shader_frag.shader.get(), "main" } };

	array vertexBinding{
		vk::VertexInputBindingDescription{ {}, sizeof(vertex_input), vk::VertexInputRate::eVertex } };

	array vertexInputs{
		vk::VertexInputAttributeDescription{ 0, 0, vk::Format::eR32G32B32A32Sfloat, 0 },
		vk::VertexInputAttributeDescription{ 1, 0, vk::Format::eR32G32B32A32Sfloat, offsetof(vertex_input, col) } };

	vk::PipelineVertexInputStateCreateInfo vertexInputState{ {},
		0, nullptr,
		0, nullptr };

	vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState{ {},
		vk::PrimitiveTopology::eTriangleList, false };

	vk::PipelineRasterizationStateCreateInfo rasterizationState{ {},
		false, false,
		vk::PolygonMode::eFill, vk::CullModeFlagBits::eNone, vk::FrontFace::eCounterClockwise,
		false, 0, 0, 0 };

	array attachments{
		vk::PipelineColorBlendAttachmentState{ false,
			 {}, {}, {},
			 {}, {}, {} } };

	vk::PipelineColorBlendStateCreateInfo blendState{ {}, false, {},
		 attachments.size(), attachments.data(),
		 { 1.0f, 1.0f, 1.0f, 1.0f } };

	array dynamic_states{
		vk::DynamicState::eViewport,
		vk::DynamicState::eScissor
	};

	vk::PipelineDynamicStateCreateInfo dynamicState{ {},
		dynamic_states.size(), dynamic_states.data() };

	vk::GraphicsPipelineCreateInfo graphicsPipelineCreate{};

	graphicsPipelineCreate.stageCount           = shaders.size();
	graphicsPipelineCreate.pStages              = shaders.data();

	graphicsPipelineCreate.pVertexInputState    = &vertexInputState;
	graphicsPipelineCreate.pInputAssemblyState  = &inputAssemblyState;
	graphicsPipelineCreate.pRasterizationState  = &rasterizationState;
	graphicsPipelineCreate.pColorBlendState     = &blendState;
	graphicsPipelineCreate.pDynamicState        = &dynamicState;

	graphicsPipelineCreate.layout               = pipelineLayout.get();
	graphicsPipelineCreate.renderPass           = renderPass.get();
	graphicsPipelineCreate.subpass              = 0;

	graphicsPipeline = context._device->createGraphicsPipelineUnique(nullptr, graphicsPipelineCreate);
}

void Renderer::create_pipeline_bindings () {
}

float __color__ = 0;

void Renderer::update() {
	__color__ += 0.01;
}

void Renderer::frame() {

	auto frame = swapChain->getFrame();

	{ VulkanFenceLock lock(context._device.get(), fence.get());

		auto& image   = frame.get().image;
		auto& buffer  = clearBuffer[frame.get().index].get();

		{ VulkanBufferRecorder record{ buffer, vk::CommandBufferUsageFlagBits::eOneTimeSubmit };

			float constcol = std::sin(__color__);
			float xonstcol = std::cos(__color__);

			auto color     = vk::ClearColorValue(array{ constcol, 1.0f - constcol, xonstcol, 1.0f });
			auto frameSize = swapChain->current_size;

			vk::RenderPassBeginInfo beginInfo{ renderPass.get(), framebuffers[frame.get().index].get()};

			beginInfo.renderArea      = vk::Rect2D{ {}, { (uint32_t) frameSize.x(), (uint32_t) frameSize.y() }};
			beginInfo.clearValueCount = 1;
			beginInfo.pClearValues    = array{ vk::ClearValue(color) }.data();

		//record->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout.get(), 0, { }, { });

			record->beginRenderPass   (beginInfo, vk::SubpassContents::eInline);

			record->bindPipeline      (vk::PipelineBindPoint::eGraphics, graphicsPipeline.get());

			record->setViewport       (0, array{ vk::Viewport{ 0, 0, float(frameSize.x()), float(frameSize.y()), 0.1, 100 } });
			record->setScissor        (0, array{ beginInfo.renderArea });

			record->draw              (3, 1, 0, 0);

			record->endRenderPass     ();
		}

		const auto rp = vector{ presentReadySemaphore.get() };
		graphicsQueue->submit({ buffer }, VulkanGraphicsQueue::GPUWaitType::semaphores({ }, rp), lock.fence);

		const auto sc = vector{ frame };
		const auto sp = vector{ presentReadySemaphore.get(), swapChain->semaphore.get() };

		graphicsQueue->present(sc, VulkanGraphicsQueue::GPUWaitType::semaphores(sp, { }));

	} // wait for frame to finish before encoding next frame: VulkanFenceLock
}
