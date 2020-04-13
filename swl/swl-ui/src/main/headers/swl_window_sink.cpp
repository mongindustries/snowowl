//
// Created by Michael Ong on 12/4/20.
//
#include "swl_window_sink.hpp"
#include "swl_window_backend.hpp"

using namespace swl::cx;
using namespace swl::ui;
using namespace backend;

void WindowSink::Sizing(bool value) {

	for (const auto& item : WindowBackend::backend->activeNativeHandles) {
		auto* window = const_cast<Window*>(std::get<0>(item));

		if (window->_handle == handle) {
			window->_resizing = value;
		}
	}
}

void WindowSink::Update(const Rect &new_rect) {

	for (const auto &item : WindowBackend::backend->activeNativeHandles) {
		auto* window = const_cast<Window*>(std::get<0>(item));

		if (window->_handle == handle) {
			window->_frame = new_rect;

			for (const auto &event : window->_event_size_list) {
				event.invoke(*window, new_rect);
			}

			break;
		}
	}
}

void WindowSink::Closed() {

	auto &handles = WindowBackend::backend->activeNativeHandles;

	for (const auto &item : handles) {

		const Window *window = item.first;

		if (window->_handle == handle) {

			for (const auto &event : window->_event_close_list) {
				event.invoke(*window);
			}

			WindowBackend::backend->RemoveEntry(window);

			break;
		}
	}
}
