//
// Created by Michael Ong on 21/4/20.
//
#pragma once

#include <vector>

#include <header.hpp>

#include "entity.hpp"
#include "layer.hpp"

#include "graph_node_argument.hpp"

SNOW_OWL_NAMESPACE(rd::graph)

struct node;

template<typename NodeType>
concept is_node = std::is_base_of_v<node, std::decay_t<NodeType>>;

template<typename NodeType>
struct node_reference {

	typedef std::enable_if_t<is_node<NodeType>, cx::exp::ptr_ref<std::decay_t <NodeType>>> Reference;
	
	cx::driver_handle  unique_id;
	Reference          reference;
};

struct node_argument_state {
};

struct node_context {

	typedef std::pair<cx::exp::ptr_ref<entity>, std::vector<cx::exp::ptr_ref<node_effect>>> EntityEffectContext;

	cx::exp::ptr_ref<rd::layer> const                   layer;

	std::vector<EntityEffectContext> const              entites;

	std::vector<cx::exp::ptr_ref<node_argument_state>>  inputs;
};

/**
 * A rendering Node.
 */
struct node {

	node             (const node&) = delete;

	node& operator=  (const node&) = delete;


	node             (node&& mov) noexcept = default;

	node& operator=  (node&& mov) noexcept = default;

	
	node          () = default;

	virtual ~node () = default;


	[[nodiscard]] virtual std::vector<node_argument>
				input_arguments   () const { return { }; }

	[[nodiscard]] virtual std::vector<node_argument>
				output_arguments  () const { return { }; }


	/**
	 * \brief Creates this node's persistent render resource.
	 * \return A \code cx::exp::ptr_ref<node_argument_state> \endcode to the node's resource.
	 */
	[[nodiscard]] virtual std::vector<cx::exp::ptr<node_argument_state>>
				create_resource   () const { return { }; }

	/**
	 * \brief The list of entity effect this node supports.
	 * \returns A \code cx::driver_handle \endcode list of effects.
	 */
	[[nodiscard]] virtual std::vector<cx::driver_handle>
				effects           () const { return { }; }

	/**
	 * \brief Prepares this node for the current render pass.
	 * \param context The render context for this ingest stage.
	 */
	virtual void
				ingest_data       (const node_context& context) { }

	virtual void
				render_data       (const node_context& context) { }
};

SNOW_OWL_NAMESPACE_END
