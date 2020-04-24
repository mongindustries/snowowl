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

SNOW_OWL_NAMESPACE(rd)

enum class layer_type {
	typeOnScene,
	typeOffScene
};

struct entity_not_found: std::exception {
};

struct layer {

	template<typename EntityType>
	struct entity_reference {

		typedef std::enable_if_t<is_entity<EntityType>, cx::exp::ptr_ref<std::decay_t<EntityType>>> Reference;

		cx::driver_handle  unique_id;
		Reference          reference;
	};


	layer             (const layer&) = delete;

	layer& operator=  (const layer&) = delete;

	
	layer             (layer&& mov) noexcept;

	layer& operator=  (layer&& mov) noexcept;


	layer             (const std::string& name, layer_type layer_type);

	virtual ~layer    ();


	template<typename EntityType> requires is_entity<EntityType>
	entity_reference<EntityType>
				add_entity           (EntityType&& entity);

	template<typename EntityType> requires is_entity<EntityType>
	void  add_entity_static    (EntityType&& entity);

	template<typename EntityType> requires is_entity<EntityType>
	entity_reference<EntityType>
				find_entity_by_name  (const std::string& name);

	void  bind_renderer        (const cx::driver_handle& reference);


	layer_camera                      camera{};

	cx::driver_handle                 bound_renderer{};

	std::vector<cx::exp::ptr<entity>> entities;
};


template<typename EntityType> requires is_entity<EntityType>
layer::entity_reference<EntityType>
			layer::add_entity         (EntityType &&entity) {

	using TypeNoRef = std::decay_t <EntityType>;

	cx::exp::ptr<rd::entity, TypeNoRef> obj { std::forward<TypeNoRef>(entity) };
	cx::exp::ptr_ref<TypeNoRef>         ref { obj };

	entities.emplace_back(obj.abstract_and_release());

	return entity_reference<EntityType>{cx::core::make_handle(), ref };
}


template<typename EntityType> requires is_entity<EntityType>
void  layer::add_entity_static  (EntityType &&entity) {

	using TypeNoRef = std::decay_t <EntityType>;

	entities.emplace_back(cx::exp::ptr<rd::entity, TypeNoRef>{ std::forward<TypeNoRef>(entity) }.abstract_and_release());
}

template<typename EntityType> requires is_entity<EntityType>
layer::entity_reference<EntityType>
			layer::find_entity_by_name(const std::string& name) {

	for (const auto& entity : entities) {
		if (entity->name == name) {
			return entity_reference<EntityType> {
				cx::core::make_handle(),
				cx::exp::ptr_ref{ entity.reify_pointer<EntityType>() } };
		}
	}

	throw entity_not_found();
}

SNOW_OWL_NAMESPACE_END
