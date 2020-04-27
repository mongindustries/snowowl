//
// Created by Michael Ong on 21/4/20.
//
#pragma once

#include <header.hpp>
#include <ownership_exp.hpp>
#include <core.hpp>

SNOW_OWL_NAMESPACE(rd)

struct entity;

SNOW_OWL_NAMESPACE_END

SNOW_OWL_NAMESPACE(rd::graph)

/**
 * Defines an effect identifier for a renderer node that will be
 * used to mark entities for inclusion on a node's render-pass.
 */
struct node_effect {
	virtual ~node_effect() = default;

	unsigned int id;

	node_effect(const unsigned int id): id(id) { }

	[[nodiscard]] virtual bool
				is_required() const { return true; }
};

template<typename NodeEffectType>
concept is_node_effect = std::is_base_of_v<node_effect, std::remove_reference_t<node_effect>>;

template<typename NodeEffectType>
struct node_effect_reference {

	typedef std::enable_if_t<is_node_effect<NodeEffectType>, cx::exp::ptr_ref<std::decay_t<NodeEffectType>>> Reference;

	cx::driver_handle        unique_id{ 0 };
	Reference                reference;

	cx::exp::ptr_ref<entity> entity_reference;
};

SNOW_OWL_NAMESPACE_END
