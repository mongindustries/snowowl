#include "renderer.hpp"
#include "rendererQueue.hpp"

namespace SWL {
	using namespace swl::cx;
	using namespace swl::ui;
	using namespace swl::gx;
}

using namespace std;

using namespace SWL;
using namespace swl::app;

auto findQueueFamily(const vector<vk::QueueFamilyProperties> &list, vk::QueueFlagBits where) -> uint32_t {

	auto item = find_if(list.begin(), list.end(), [&where](const vk::QueueFamilyProperties &property) {
		return property.queueFlags & where;
	});

	return distance(list.begin(), item);
}

Renderer::Renderer(const gx::implem::VulkanGraphicsContext& context, const Window& target_window):
	_instance(const_cast<vk::Instance&>     (context.getInstance())),
	_device(const_cast<vk::PhysicalDevice&> (context.getActiveDevice())),
	_surface                                (target_window.getSurface()) {


		context.makeSurface(_surface);

		auto vk_surface    = context.getSurface(_surface);
		auto families      = _device.getQueueFamilyProperties();

		vk::DeviceQueueCreateInfo graphicsQueue; graphicsQueue.queueFamilyIndex = -1;
		vk::DeviceQueueCreateInfo presentQueue;  presentQueue.queueFamilyIndex = -1;

		uint32_t index = 0;
		float priority = 1.0f;
		for (const auto &item : families) {

			if (item.queueFlags & vk::QueueFlagBits::eGraphics) {
				graphicsQueue.queueFamilyIndex = index;
				graphicsQueue.queueCount = 1;
				graphicsQueue.pQueuePriorities = &priority;
			}

			if (_device.getSurfaceSupportKHR(index, vk_surface)) {
				presentQueue.queueFamilyIndex = index;
				presentQueue.queueCount = 1;
				presentQueue.pQueuePriorities = &priority;
			}
		}

		vk::DeviceCreateInfo deviceCreate;

		deviceCreate.queueCreateInfoCount = 2;
		deviceCreate.pQueueCreateInfos    = array<vk::DeviceQueueCreateInfo, 2> { graphicsQueue, presentQueue }.data();

		_logicalDevice = _device.createDevice(deviceCreate);

		_graphicsQueue = RendererQueue(_logicalDevice.getQueue(graphicsQueue.queueFamilyIndex, 0));
		_presentQueue = RendererQueue(_logicalDevice.getQueue(presentQueue.queueFamilyIndex, 0));
	}
