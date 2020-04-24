//
// Created by Michael Ong on 21/4/20.
//
#include "world.hpp"
#include "world_renderer.hpp"

using namespace swl;
using namespace std;

cx::driver_handle
			rd::world::add_renderer  (world_renderer &&graph) {
	auto result = renderers.emplace(cx::core::make_handle(), std::forward<rd::world_renderer>(graph));

	if (get<1>(result)) {
		const auto& item = *get<0>(result);
		get<1>(item)->compile();

		return get<0>(item);
	}

	return {};
}

void  swl::rd::world::update   (const chrono::milliseconds &delta) {

}

void  swl::rd::world::render   (float frame_offset) {

}
