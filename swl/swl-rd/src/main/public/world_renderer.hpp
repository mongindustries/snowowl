//
// Created by Michael Ong on 21/4/20.
//
#pragma once

#include <map>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

#include <core.hpp>
#include <header.hpp>
#include <ownership_exp.hpp>

#include "layer.hpp"

#include "graph_node.hpp"
#include "graph_node_argument.hpp"

SNOW_OWL_NAMESPACE(rd)

struct invalid_linkage: std::exception {
};

struct world_renderer_state {

	typedef std::map<cx::driver_handle, std::vector<cx::exp::ptr<graph::node_argument_input>>> state_input;


	world_renderer_state             (const world_renderer_state& cpy) = delete;

	world_renderer_state& operator=  (const world_renderer_state&) = delete;

	
	world_renderer_state             (world_renderer_state&& mov) noexcept;

	world_renderer_state& operator=  (world_renderer_state&& mov) noexcept;


	world_renderer_state   ();

	~world_renderer_state  ();


	state_input inputs;
};

struct world_renderer final {

	struct _node_graph {

		cx::driver_handle                          node;

		std::vector<cx::exp::ptr_ref<_node_graph>> parent;
		std::vector<cx::exp::ptr<_node_graph>>     children;

		_node_graph             (_node_graph &&mov) noexcept;

		_node_graph& operator=  (_node_graph &&mov) noexcept;


		_node_graph                   (const _node_graph& cpy) = delete;

		_node_graph const& operator=  (const _node_graph& cpy) = delete;
		

		_node_graph(
			cx::driver_handle                          node,
			std::vector<cx::exp::ptr_ref<_node_graph>> parent,
			std::vector<cx::exp::ptr<_node_graph>>     children);

		~_node_graph();


		bool operator==(cx::driver_handle rhs) const;

		friend struct world_renderer;

	private:

		[[nodiscard]] std::vector<cx::exp::ptr_ref<_node_graph>>
					find_leaf ();

		[[nodiscard]] cx::exp::ptr_ref<_node_graph>
					find      (cx::driver_handle handle);
	};


	world_renderer             (const world_renderer&) = delete;

	world_renderer& operator=  (const world_renderer& cpy) = delete;
	

	world_renderer             (world_renderer &&mov) noexcept;

	world_renderer& operator=  (world_renderer&& mov) noexcept;
	

	~world_renderer();


	explicit world_renderer (std::string name);


	template<typename NodeType> requires graph::is_node<NodeType>
	graph::node_reference<NodeType>
				add_node    (NodeType &&node);

	template<typename NodeType> requires graph::is_node<NodeType>
	void  set_root    (const graph::node_reference<NodeType> &root);

	template<typename PNodeType, typename CNodeType> requires graph::is_node<PNodeType> && graph::is_node<CNodeType>
	void  add_link    (
		const graph::node_reference<PNodeType>&                                   parent,
		const graph::node_reference<CNodeType>&                                   child,
		const std::vector<std::pair<graph::node_argument, graph::node_argument>>& linkages);

	template<typename NodeType> requires graph::is_node<NodeType>
	void  set_output  (
		const graph::node_reference<NodeType>&  node,
		const graph::node_argument&             node_output);


	void  compile     (std::vector<cx::exp::ptr_ref<layer>>& layers);

	void  step        (const std::vector<cx::exp::ptr_ref<layer>>& layers);

	void  render      (const std::vector<cx::exp::ptr_ref<layer>>& layers);


	std::string                                             name;

	std::map<cx::driver_handle, cx::exp::ptr<graph::node>>  nodes;

	cx::exp::ptr<_node_graph>                               tree{ nullptr };

	std::vector<cx::exp::ptr<world_renderer_state>>         state;
};

template<typename NodeType> requires graph::is_node<NodeType>
graph::node_reference<NodeType>
	world_renderer::add_node           (NodeType &&node) {

	using TypeNoRef = std::remove_reference_t<NodeType>;

	cx::exp::ptr<graph::node, TypeNoRef>  obj { std::forward<TypeNoRef>(node) };
	cx::exp::ptr_ref<TypeNoRef>           ref { obj };

	cx::driver_handle handle = cx::core::make_handle();
	nodes.emplace(handle, obj.abstract_and_release());

	return graph::node_reference<NodeType> {handle, ref };
}

template<typename NodeType> requires graph::is_node<NodeType>
void world_renderer::set_root   (const graph::node_reference<NodeType> &root) {
	tree = cx::exp::ptr<_node_graph>{ _node_graph(root.unique_id, { }, std::vector<cx::exp::ptr<_node_graph>>{ }) };
}

template<typename PNodeType, typename CNodeType> requires graph::is_node<PNodeType> && graph::is_node<CNodeType>
void world_renderer::add_link   (
	const graph::node_reference<PNodeType>&                                   parent,
	const graph::node_reference<CNodeType>&                                   child,
	const std::vector<std::pair<graph::node_argument, graph::node_argument>>& linkages) {

	PNodeType& pref = parent.reference;
	CNodeType& cref = child.reference;

	for (const auto& param : linkages) {
		const auto& from  = get<0>(param);
		const auto& to    = get<1>(param);

		std::vector<graph::node_argument> output  = pref.output_arguments();
		std::vector<graph::node_argument> input   = cref.input_arguments();

		std::vector<bool> checks {
			std::find(output.begin(), output.end(), from) != output.end(),
			std::find(input.begin(),  input.end(),  to  ) != input.end(),
			from.slot == to.slot,
		};

		if (std::find(checks.begin(), checks.end(), false) != checks.end()) {
			throw invalid_linkage();
		}
	}

	cx::exp::ptr_ref<_node_graph> node = tree->find(parent.unique_id);

	if (node) {
		node->children.emplace_back(_node_graph(child.unique_id, {}, {}));
	}
}

template<typename NodeType> requires graph::is_node<NodeType>
void world_renderer::set_output (
	const graph::node_reference<NodeType>&  node,
	const graph::node_argument&             node_output) {

	(void) tree;
}

SNOW_OWL_NAMESPACE_END
