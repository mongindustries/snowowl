#include <application.hpp>

#include <graphicsCanvas.hpp>
#include <vulkanGraphicsContext.hpp>

#include "renderer.hpp"

using namespace swl;
using namespace cx;
using namespace ui;

struct App: Application {

	WindowSurface surface;

	gx::GraphicsCanvas<gx::implem::VulkanGraphicsContext> canvas{ gx::implem::VulkanGraphicsContext() };

	Own<app::Renderer> renderer;


	explicit App(void* instance): Application(instance), renderer(nullptr) {
	}

	void applicationCreate  () override {

		const auto window = createWindow("[SnowOwl:] App", Rect{ { 100, 100 }, { 800, 480 } });

		auto& window_ref = window();

		renderer = new app::Renderer(canvas.context(), window_ref);

		const std::function<void(const Window&, const Window::State&)> event = [&, this](const Window&, const Window::State& state) -> void {
			renderer()->frame();
		};

		const std::function<void(const Window&, const Rect&)> sizeEvent = [&, this](const Window&, const Rect&) -> void {
			renderer()->frame();
		};

		window_ref._event_state_list.emplace_back(event);
		window_ref._event_size_list.emplace_back(sizeEvent);
	}

	void applicationDestroy () override {
	}
};

#if defined(SWL_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR cmdArgs, int cmdShow) {
#elif defined(SWL_DARWIN)
int main () {
	std::nullptr_t instance = nullptr; // implied [NSApplication sharedApplication];
#endif

	return Application::runApplication(App(instance));
}
