//
// Created by Michael Ong on 21/4/20.
//
#pragma once

#include <string>
#include <map>
#include <vector>
#include <functional>

#include <core.hpp>
#include <header.hpp>
#include <ownership.hpp>

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

		cx::driver_handle        unique_id{ 0 };
		Reference                reference;
	};


	layer             (const layer&) = delete;

	layer& operator=  (const layer&) = delete;

	
	layer             (layer&& mov) noexcept;

	layer& operator=  (layer&& mov) noexcept;


	layer             (const std::string& name, layer_type layer_type);

	virtual ~layer    ();


	template<typename EntityType> requires is_entity<EntityType>
	entity_reference<EntityType>
				add_entity           (EntityType&& entity, const std::function<void(EntityType&)>& configure = [](auto _) {});

	template<typename EntityType> requires is_entity<EntityType>
	void  add_entity_static    (EntityType&& entity, const std::function<void(EntityType&)>& configure = [](auto _) {});

	template<typename EntityType> requires is_entity<EntityType>
	entity_reference<EntityType>
				find_entity_by_name  (const std::string& name);

	void  bind_renderer        (const cx::driver_handle& reference);

	void  mark_dirty           ();


	layer_camera                                                        camera{};

	cx::driver_handle                                                   bound_renderer{};


	std::vector<cx::exp::ptr<entity>>                                   entities;

	std::map<cx::driver_handle, std::vector<cx::exp::ptr_ref<entity>>>  effect_entity_reference;

	friend struct world;
	friend struct world_renderer;

private:

	bool                                                                dirty;
};


template<typename EntityType> requires is_entity<EntityType>
layer::entity_reference<EntityType>
			layer::add_entity         (EntityType &&entity, const std::function<void(EntityType&)>& configure) {

	using TypeNoRef = std::decay_t <EntityType>;

	cx::exp::ptr<rd::entity, TypeNoRef> obj { std::forward<TypeNoRef>(entity) };
	cx::exp::ptr_ref<TypeNoRef>         ref { obj };

	obj->ref_layer = cx::exp::ptr_ref<layer>{ this };

	configure(obj);
	mark_dirty();

	entities.emplace_back(obj.abstract_and_release());

	return entity_reference<EntityType>{cx::core::make_handle(), ref };
}


template<typename EntityType> requires is_entity<EntityType>
void  layer::add_entity_static  (EntityType &&entity, const std::function<void(EntityType&)>& configure) {

	using TypeNoRef = std::decay_t <EntityType>;

	cx::exp::ptr<rd::entity, TypeNoRef> obj{ std::forward<TypeNoRef>(entity) };

	obj->ref_layer = cx::exp::ptr_ref<layer>{ this };

	configure(obj);
	mark_dirty();

	entities.emplace_back(obj.abstract_and_release());
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
