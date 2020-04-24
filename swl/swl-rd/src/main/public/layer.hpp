//
// Created by Michael Ong on 21/4/20.
//
#pragma once

#include <string>
#include <vector>

#include <core.hpp>
#include <header.hpp>
#include <ownership_exp.hpp>

#include "entity.hpp"
#include "layer_camera.hpp"

#include "graph_node_effect.hpp"
#include "world_renderer.hpp"

SNOW_OWL_NAMESPACE(rd)

enum class layer_type {
	typeOnScene,
	typeOffScene
};

struct entity_not_found: public std::exception {

};

struct layer {

	template<typename EntityType> requires is_entity<EntityType>
	struct entity_reference {
		cx::driver_handle                                     unique_id;
		cx::exp::ptr_ref<std::remove_reference_t<EntityType>> entity_reference;
	};


	layer           (const layer& cpy) = delete;

	layer           (layer&& mov) noexcept;


	layer           (
		const std::string&  name,
		layer_type           layerType);


	virtual ~layer  () = default;


	template<typename EntityType> requires is_entity<EntityType>
	entity_reference<EntityType>
				add_entity          (EntityType&& entity);

	template<typename EntityType> requires is_entity<EntityType>
	void  add_entity_static   (EntityType&& entity);

	template<typename EntityType> requires is_entity<EntityType>
	entity_reference<EntityType>
				find_entity_by_name (std::string name);

	void  bind_renderer       (const cx::driver_handle& reference);

	layer_camera                      camera{};

	cx::driver_handle                 bound_renderer;

	std::vector<cx::exp::ptr<entity>> entities;
};


template<typename EntityType> requires is_entity<EntityType>
layer::entity_reference<EntityType>
			layer::add_entity         (EntityType &&entity) {

	using TypeNoRef = std::remove_reference_t<EntityType>;

	cx::exp::ptr<rd::entity, TypeNoRef> obj { std::forward<TypeNoRef>(entity) };
	cx::exp::ptr_ref<TypeNoRef>         ref { obj };

	entities.emplace_back(obj.abstract_and_release());

	return layer::entity_reference<EntityType>{cx::core::make_handle(), ref };
}


template<typename EntityType> requires is_entity<EntityType>
void  layer::add_entity_static  (EntityType &&entity) {

	using TypeNoRef = std::remove_reference_t<EntityType>;
	entities.emplace_back(cx::exp::ptr<rd::entity, TypeNoRef>{ std::forward<TypeNoRef>(entity) }.abstract_and_release());
}

template<typename EntityType> requires is_entity<EntityType>
layer::entity_reference<EntityType>
			layer::find_entity_by_name(std::string name) {

	for (const cx::exp::ptr<entity>& entity : entities) {
		if (entity->name == name) {
			return layer::entity_reference<EntityType> {
				cx::core::make_handle(),
				cx::exp::ptr_ref{ entity.reify_pointer<EntityType>() } };
		}
	}

	throw entity_not_found();
}

SNOW_OWL_NAMESPACE_END
