//
// Created by Michael Ong on 21/4/20.
//
#pragma once

#include <string>
#include <vector>
#include <chrono>

#include <header.hpp>
#include <matrix.hpp>

#include "graph_node_effect.hpp"

SNOW_OWL_NAMESPACE(rd)

/**
 * An entity serves as the base class a\code layer\endcode would get buffer information from.
 */
struct entity {

	entity            (const entity&) = delete;

	entity& operator= (const entity&) = delete;
	

	entity            (entity&&) noexcept;

	entity& operator= (entity&&) noexcept;


	entity            ();

	explicit entity   (std::string name);

	virtual ~entity   () = default;

	 /**
	  * Binds a particular Node effect to this entity.
	  * @tparam NodeEffectType The target effect type.
	  * @param effect_id The Node Effect instance to add.
	  * @return A\code node_effect_reference\endcode instance containing the newly created node effect.
	  */
	template<typename NodeEffectType> requires rd::graph::is_node_effect<NodeEffectType>
	graph::node_effect_reference<NodeEffectType>
				mark_effect (NodeEffectType &&effect_id);

	/**
	 * Updates function for the entity.
	 * @param offset Frame time of the game.
	 */
	virtual void
				update      (std::chrono::milliseconds offset);


	std::string                                   name;

	cx::matrix<float, 4>                          transform;

	std::vector<cx::exp::ptr<graph::node_effect>> effects;
};

template<typename NodeEffectType> requires rd::graph::is_node_effect<NodeEffectType>
graph::node_effect_reference<NodeEffectType>
	entity::mark_effect(NodeEffectType &&effect_id) {

	using TypeNoReference = std::remove_reference_t<NodeEffectType>;

	cx::exp::ptr<graph::node_effect, TypeNoReference> obj { std::forward<TypeNoReference>(effect_id) };
	cx::exp::ptr_ref<TypeNoReference>                 ref { obj };

	effects.emplace_back(obj.abstract_and_release());

	return {cx::core::make_handle(), ref };
}

template<typename EntityType>
concept is_entity = std::is_base_of_v<entity, std::decay_t<EntityType>>;

SNOW_OWL_NAMESPACE_END
