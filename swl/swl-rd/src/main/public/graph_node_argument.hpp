//
// Created by Michael Ong on 21/4/20.
//
#pragma once

#include <vector>

#include <header.hpp>

SNOW_OWL_NAMESPACE(rd::graph)

struct node_argument_type {
	static node_argument_type typeTexture2D;
	static node_argument_type typeTexture3D;

	static node_argument_type typeDataBuffer;

	unsigned int id;

	bool operator==(const node_argument_type& rhs) const {
		return id == rhs.id;
	}
};

enum struct node_argument_scope {
	scopePerLayer,
	scopePerGraph
};

/**
 * Defines a Node's argument that will be used on the succeeding
 * Node's input.
 *
 */
struct node_argument {

	unsigned int        id;

	bool                required;
	bool                can_display;

	node_argument_type  slot;
	node_argument_scope scope;

	explicit node_argument(
		unsigned            id,
		bool                required,
		bool                can_display,
		node_argument_type  slot,
		node_argument_scope scope) noexcept:
			id          (id),
			required    (required),
			can_display (can_display),
			slot        (slot),
			scope       (scope) { }

	bool operator==(const node_argument &rhs) const {
		return id == rhs.id;
	}
};

SNOW_OWL_NAMESPACE_END
