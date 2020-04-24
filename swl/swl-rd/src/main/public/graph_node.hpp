//
// Created by Michael Ong on 21/4/20.
//
#pragma once

#include <string>
#include <vector>

#include <header.hpp>

#include "entity.hpp"
#include "graph_node_argument.hpp"

SNOW_OWL_NAMESPACE(rd::graph)

struct node;

template<typename NodeType>
concept is_node = std::is_base_of_v<node, std::remove_reference_t<NodeType>>;

template<typename NodeType> requires graph::is_node<NodeType>
struct node_reference {
	cx::driver_handle                                   unique_id;
	cx::exp::ptr_ref<std::remove_reference_t<NodeType>> reference;
};

struct node_argument_input {
};

/**
 * A rendering Node.
 */
struct node {

	node          () = default;

	virtual ~node () = default;


	[[nodiscard]] virtual std::vector<cx::driver_handle>
				effects                 () const { return { }; }

	[[nodiscard]] virtual std::vector<node_argument>
				input_arguments         () const { return { }; }

	[[nodiscard]] virtual std::vector<node_argument>
				output_arguments        () const { return { }; }


	[[nodiscard]] virtual std::vector<cx::exp::ptr_ref<node_argument_input>>
				build_output_arguments  (const std::vector<cx::exp::ptr_ref<entity>>& entities) const { return { }; }


	[[nodiscard]] virtual bool
				shall_upload_resources  (const std::vector<cx::exp::ptr_ref<node_argument_input>> &inputs) const { return false; }

	virtual void
				upload_resources        (const std::vector<cx::exp::ptr_ref<node_argument_input>> &inputs) { }


	[[nodiscard]] virtual bool
				shall_record_commands   (const std::vector<cx::exp::ptr_ref<node_argument_input>> &inputs) const { return false; }

	virtual void
				record_commands         (const std::vector<cx::exp::ptr_ref<node_argument_input>> &inputs) { }
};

SNOW_OWL_NAMESPACE_END
