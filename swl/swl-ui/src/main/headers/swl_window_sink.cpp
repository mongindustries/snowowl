//
// Created by Michael Ong on 12/4/20.
//
#include "swl_window_sink.hpp"
#include "swl_window_backend.hpp"

using namespace swl::cx;

SNOW_OWL_NAMESPACE(ui)

using namespace backend;

void window_sink::sizing(bool value) {

	for (const auto& item : window_backend::instance->native_handles) {
		auto* window = const_cast<ui::window*>(std::get<0>(item));

		if (window->handle == handle) {
			window->resizing = value;

			if (!window->is_sizing()) {
				window->loop_wait.notify_all();
			}
		}
	}
}

void window_sink::frame(const rect &new_rect) {

	for (const auto &item : window_backend::instance->native_handles) {
		auto* window = const_cast<ui::window*>(std::get<0>(item));

		if (window->handle == handle) {
			window->frame = new_rect;

			for (const auto &event : window->event_on_resize) {
				event.invoke(*window, new_rect);
			}

			break;
		}
	}
}

void window_sink::closed() {

	auto &handles = window_backend::instance->native_handles;

	for (const auto &item : handles) {

		const window *window = item.first;

		if (window->handle == handle) {

			for (const auto &event : window->event_on_close) {
				event.invoke(*window);
			}

			window_backend::instance->destroy(window);
			return;
		}
	}
}

SNOW_OWL_NAMESPACE_END
