//
// Created by Michael Ong on 21/4/20.
//
#include <world_renderer.hpp>
#include <utility>

using namespace swl;
using namespace std;

SNOW_OWL_NAMESPACE(rd)

world_renderer_state::world_renderer_state   () = default;

world_renderer_state::~world_renderer_state  () = default;


world_renderer_state::world_renderer_state
	(world_renderer_state&& mov) noexcept:
 inputs(std::move(mov.inputs)){ }

world_renderer_state& world_renderer_state::operator=(world_renderer_state&& mov) noexcept {
	inputs = std::move(mov.inputs);

	return *this;
}


world_renderer::_node_graph::_node_graph
	(cx::driver_handle node, std::vector<cx::exp::ptr_ref<_node_graph>> parent, std::vector<cx::exp::ptr<_node_graph>> children):
	node(node), parent(std::move(parent)), children(std::move(children)) {}

world_renderer::_node_graph::~_node_graph
	() = default;

world_renderer::_node_graph::_node_graph
	(_node_graph &&mov) noexcept:
	node(mov.node), children(std::move(mov.children)) { }

bool  world_renderer::_node_graph::operator== (cx::driver_handle rhs) const {
	return node == rhs;
}


world_renderer::_node_graph& 
			world_renderer::_node_graph::operator=  (_node_graph&& mov) noexcept {

	node      = mov.node;
	mov.node  = 0;

	parent    = std::move(mov.parent);
	children  = std::move(mov.children);

	return *this;
}

std::vector<cx::exp::ptr_ref<world_renderer::_node_graph>>
			world_renderer::_node_graph::find_leaf  () {

	if (children.empty()) {
		return { cx::exp::ptr_ref{ this } };
	}

	std::vector<cx::exp::ptr_ref<_node_graph>> fill{};

	for (auto& item : children) {
		auto result = item->find_leaf();
		fill.insert(fill.end(), result.begin(), result.end());
	}

	return fill;
}

cx::exp::ptr_ref<world_renderer::_node_graph>
			world_renderer::_node_graph::find       (const cx::driver_handle handle) {

	if (node == handle) {
		return cx::exp::ptr_ref<_node_graph>{this };
	}

	for (auto& item : children) {
		auto result = item->find(handle);

		if (result) {
			return result;
		}
	}

	return cx::exp::ptr_ref<_node_graph>(nullptr);
}


world_renderer::world_renderer
	(string name):
	name(std::move(name)) { }

world_renderer::world_renderer
	(world_renderer&& mov) noexcept :
	name(std::move(mov.name)), nodes(std::move(mov.nodes)), tree(std::move(mov.tree)) { }

world_renderer::~world_renderer
	() = default;


world_renderer& 
			world_renderer::operator=  (world_renderer&& mov) noexcept {

	name  = std::move(mov.name);
	nodes = std::move(mov.nodes);
	tree  = cx::exp::ptr<_node_graph> { mov.tree.release() };
	state = std::move(mov.state);

	return *this;
}


void  world_renderer::compile    (std::vector<cx::exp::ptr_ref<layer>>& layers) {

	// prepare initial render resources for all layers. 90% of the time, a world would only have
	// one world_renderer attached to it. resource for all nodes are created per-layer for
	// concurrency purposes.

	for (const auto& layer : layers) {

		(void)layer;

		cx::exp::ptr<world_renderer_state> state{ world_renderer_state{ } };

		for (const auto& item : nodes) {
			state->inputs.emplace(item.first, get<1>(item)->create_resource());
		}

		this->state.emplace_back(::move(state));
	}
}

void  world_renderer::step       (const std::vector<cx::exp::ptr_ref<layer>>& layers) {

	// TODO: add a caching state for entity filter and input construction (input most likely be after create resource).

	unsigned int layer_index = 0;
	for (const auto& layer : layers) {

		auto batch = std::vector{ cx::exp::ptr_ref<_node_graph>{ tree } };

		while (batch.empty() == false) {

			vector<cx::exp::ptr_ref<_node_graph>> next_batch{};
			next_batch.reserve(10); // conservative guess

			for (auto& graph_item : batch) {

				auto node = cx::exp::ptr_ref<graph::node>{ nodes.at(graph_item->node) };
				auto effects = node->effects();

				vector<graph::node_context::EntityEffectContext> filtered_entities;

				// find entities that match this node's requirement
				for (const auto& entity : layer->entities) {

					vector<cx::exp::ptr_ref<graph::node_effect>> staged_effects{};

					// fill the effects this node is going to use for this entity
					for (const auto& effect : entity->effects) {
						if (std::find(effects.begin(), effects.end(), effect->id) != effects.end()) {
							staged_effects.emplace_back(effect);
						}
					}

					if (!staged_effects.empty()) {
						filtered_entities.emplace_back(entity, staged_effects);
					}
				}

				auto &input_ref = state[layer_index++]->inputs.at(graph_item->node);

				vector<cx::exp::ptr_ref<graph::node_argument_state>> inputs{ };
				inputs.reserve(input_ref.size());

				std::transform(
					input_ref  .begin(),
					input_ref   .end(),
					inputs      .begin(), [](const auto& item) { return cx::exp::ptr_ref<graph::node_argument_state>{ item }; });

				node->ingest_data(graph::node_context{
					layer, filtered_entities, inputs
				});

				vector<cx::exp::ptr_ref<_node_graph>> ref_children{};
				ref_children.reserve(graph_item->children.size());

				transform(
					graph_item->children.begin(),
					graph_item->children.end(),
					ref_children.begin(),
					[](const auto& item) { return cx::exp::ptr_ref<_node_graph>{ item }; }
				);

				next_batch.insert(next_batch.end(), ref_children.begin() , ref_children.end());
			}
		}
	}
}

void  world_renderer::render     (const std::vector<cx::exp::ptr_ref<layer>>& layers) {

}

SNOW_OWL_NAMESPACE_END
