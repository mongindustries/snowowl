#include "renderer.hpp"

#include "rendererBufferBasicClear.hpp"
#include "rendererQueue.hpp"

namespace SWL {
	using namespace swl::cx;
	using namespace swl::ui;
	using namespace swl::gx;
}

using namespace std;

using namespace SWL;
using namespace swl::app;

auto FindQueueFamily(const vector<vk::QueueFamilyProperties> &list, vk::QueueFlagBits where) -> uint32_t {

	const auto item = find_if(list.begin(), list.end(), [&where](const vk::QueueFamilyProperties &property) {
		return property.queueFlags & where;
	});

	return distance(list.begin(), item);
}

Renderer::Renderer(const implem::VulkanGraphicsContext& context, const Window& target_window) :
	instance(const_cast<vk::Instance&>(context.getInstance())),
	physical_device(const_cast<vk::PhysicalDevice&>(context.getActiveDevice())),

	graphics_queue(nullptr),
	present_queue(nullptr),

	swapchain(nullptr),

	commands(nullptr) {

	auto window_surface = target_window.getSurface();
	
	context.makeSurface(window_surface);
	surface = context.getSurface(window_surface);

	auto families = physical_device.getQueueFamilyProperties();

	vk::DeviceQueueCreateInfo graphicsQueue; graphicsQueue.queueFamilyIndex = -1;
	vk::DeviceQueueCreateInfo presentQueue;  presentQueue.queueFamilyIndex = -1;

	uint32_t index = 0;
	auto priority = 1.0f;
	for (const auto& item : families) {

		if (item.queueFlags & vk::QueueFlagBits::eGraphics) {
			graphicsQueue.queueFamilyIndex = index;
			graphicsQueue.queueCount = 1;
			graphicsQueue.pQueuePriorities = &priority;
		}

		if (physical_device.getSurfaceSupportKHR(index, surface)) {
			presentQueue.queueFamilyIndex = index;
			presentQueue.queueCount = 1;
			presentQueue.pQueuePriorities = &priority;
		}

		index += 1;
	}

	vk::DeviceCreateInfo deviceCreate;

	array<vk::DeviceQueueCreateInfo, 2> queues = {
		graphicsQueue,
		presentQueue
	};
	
	deviceCreate.queueCreateInfoCount = queues.size();
	deviceCreate.pQueueCreateInfos    = queues.data();

	array<const char*, 1> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	deviceCreate.enabledExtensionCount   = deviceExtensions.size();
	deviceCreate.ppEnabledExtensionNames = deviceExtensions.data();

	logical_device = physical_device.createDeviceUnique(deviceCreate);
	auto& device   = logical_device.get();

	const vk::SemaphoreCreateInfo createSemaphore;
	frameSemaphore = logical_device->createSemaphoreUnique(createSemaphore);
	
	graphics_queue = new RendererQueue(graphicsQueue.queueFamilyIndex, device);
	present_queue  = new RendererQueue(presentQueue.queueFamilyIndex,  device);

	swapchain      = new RendererSwapchain(this, target_window);

	commands       = new RendererBufferBasicClear(*graphics_queue);
}

void Renderer::frame() {

	const auto &swapchain = *(this->swapchain);
	auto frame = swapchain.getNextImage(frameSemaphore.get());

	auto clear = static_cast<RendererBufferBasicClear*>(commands());

	clear->imageToClear = &get<1>(frame);
	clear->execute();

	graphics_queue()->submit(*clear);

	swapchain.present(present_queue()->queue, get<0>(frame), frameSemaphore.get());
}
