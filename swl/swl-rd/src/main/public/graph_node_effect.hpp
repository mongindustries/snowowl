//
// Created by Michael Ong on 21/4/20.
//
#pragma once

#include <header.hpp>
#include <ownership_exp.hpp>
#include <core.hpp>

SNOW_OWL_NAMESPACE(rd::graph)

/**
 * Defines an effect identifier for a renderer node that will be
 * used to mark entities for inclusion on a node's render-pass.
 */
struct node_effect {

	unsigned int id;

	node_effect(unsigned int id): id(id) { }

	[[nodiscard]] virtual bool
				is_required() const { return true; }
};

template<typename NodeEffectType>
concept is_node_effect = std::is_base_of_v<node_effect, std::remove_reference_t<node_effect>>;

template<typename NodeEffectType> requires is_node_effect<NodeEffectType>
struct node_effect_reference {

	swl::cx::driver_handle                                           unique_id;
	swl::cx::exp::ptr_ref<std::remove_reference_t<NodeEffectType>>  reference;
};

SNOW_OWL_NAMESPACE_END