#include "renderer.hpp"

namespace SWL {
	using namespace swl::cx;
	using namespace swl::ui;
	using namespace swl::gx;
}

using namespace SWL;
using namespace swl::app;

Renderer::Renderer(const gx::implem::VulkanGraphicsContext& context, const Window& target_window):
	_instance(const_cast<vk::Instance&>  (context.getInstance())),
	_device(const_cast<vk::Device&>      (context.getActiveDevice())),
	_surface                             (target_window.getSurface()) {
	context.makeSurface(_surface);
}
