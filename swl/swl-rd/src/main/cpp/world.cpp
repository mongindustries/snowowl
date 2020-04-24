//
// Created by Michael Ong on 21/4/20.
//
#include "world.hpp"
#include "world_renderer.hpp"

using namespace swl;
using namespace std;

SNOW_OWL_NAMESPACE(rd)

cx::driver_handle
			world::add_renderer  (world_renderer &&graph) {
	auto result = renderers.emplace(cx::core::make_handle(), std::forward<rd::world_renderer>(graph));

	if (get<1>(result)) {
		const auto& item = *get<0>(result);

		vector<cx::exp::ptr_ref<layer>> layers{ active_layers.size(), cx::exp::ptr_ref<layer>{ nullptr } };
		std::transform(active_layers.begin(), active_layers.end(), layers.begin(), [](auto& item) {
			return cx::exp::ptr_ref<layer>{ item };
		});

		get<1>(item)->compile(layers);

		return get<0>(item);
	}

	return {};
}

void  world::update   (const chrono::milliseconds &delta) {

	for (const auto& layer : active_layers) {
		for (const auto& entity : layer->entities) {
			entity->update(delta);
		}
	}

	std::vector<cx::exp::ptr_ref<layer>> layers;
	layers.reserve(active_layers.size());

	std::transform(active_layers.begin(), active_layers.end(), layers.begin(), [](auto& item) {
		return cx::exp::ptr_ref<layer>{ item };
	});
	
	for (const auto& renderer : renderers) {
		renderer.second->step(layers);
	}
}

void  world::render   (float frame_offset) {

	std::vector<cx::exp::ptr_ref<layer>> layers;
	layers.reserve(active_layers.size());

	std::transform(active_layers.begin(), active_layers.end(), layers.begin(), [](auto& item) {
		return cx::exp::ptr_ref<layer>{ item };
		});

	for (const auto& renderer : renderers) {
		renderer.second->render(layers);
	}
}

SNOW_OWL_NAMESPACE_END
