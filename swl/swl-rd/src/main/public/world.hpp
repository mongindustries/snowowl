//
// Created by Michael Ong on 21/4/20.
//
#pragma once

#include <chrono>
#include <vector>

#include <core.hpp>
#include <header.hpp>
#include <ownership_exp.hpp>

#include "layer.hpp"
#include "world_renderer.hpp"

SNOW_OWL_NAMESPACE(rd)

struct world {

	template<typename LayerType,
	  std::enable_if_t<std::is_base_of_v<layer, std::decay_t<LayerType>>, int> = 0>
	struct LayerReference {
		cx::driver_handle                                      unique_id;
		cx::exp::ptr_ref<std::remove_reference_t<LayerType>>  reference;
	};

	template<typename LayerType>
	LayerReference<LayerType>
				add_layer     (LayerType &&layer);

	cx::driver_handle
				add_renderer  (world_renderer &&graph);


	void  update        (const std::chrono::milliseconds &delta);

	void  render        (float frame_offset);


	std::map<cx::driver_handle, cx::exp::ptr<world_renderer>> renderers;
	std::vector<cx::exp::ptr<layer>>                          active_layers;
};

template<typename LayerType>
rd::world::LayerReference<LayerType>
	rd::world::add_layer(LayerType &&layer) {

	using TypeNoRef = std::remove_reference_t<LayerType>;

	cx::exp::ptr<rd::layer, TypeNoRef>  obj { std::forward<TypeNoRef>(layer) };
	cx::exp::ptr_ref<TypeNoRef>         ref { obj };

	active_layers.emplace_back(obj.abstract_and_release());

	return LayerReference<LayerType> {cx::core::make_handle(), ref };
}

SNOW_OWL_NAMESPACE_END
