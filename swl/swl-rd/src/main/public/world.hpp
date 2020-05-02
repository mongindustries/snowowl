//
// Created by Michael Ong on 21/4/20.
//
#pragma once

#include <chrono>
#include <vector>

#include <core.hpp>
#include <header.hpp>
#include <ownership.hpp>

#include "layer.hpp"
#include "world_renderer.hpp"

SNOW_OWL_NAMESPACE(rd)

struct world {

	template<typename LayerType>
	struct layer_reference {

		typedef std::enable_if_t<std::is_base_of_v<layer, std::decay_t<LayerType>>, cx::exp::ptr_ref<std::decay_t<LayerType>>> Reference;

		cx::driver_handle  unique_id;
		Reference          reference;
	};

	typedef std::map<cx::driver_handle, cx::exp::ptr<world_renderer>> RendererMap;
	
	template<typename LayerType>
	layer_reference<LayerType>
				add_layer     (LayerType &&layer);

	cx::driver_handle
				add_renderer  (world_renderer &&graph);


	void  update        (const std::chrono::milliseconds &delta);

	void  render        (float frame_offset);


	RendererMap                       renderers;
	std::vector<cx::exp::ptr<layer>>  active_layers;
};

template<typename LayerType>
rd::world::layer_reference<LayerType>
	rd::world::add_layer(LayerType &&layer) {

	using TypeNoRef = std::remove_reference_t<LayerType>;

	cx::exp::ptr<rd::layer, TypeNoRef>  obj { std::forward<TypeNoRef>(layer) };
	cx::exp::ptr_ref<TypeNoRef>         ref { obj };

	active_layers.emplace_back(obj.abstract_and_release());

	return layer_reference<LayerType> { cx::core::make_handle(), ref };
}

SNOW_OWL_NAMESPACE_END
