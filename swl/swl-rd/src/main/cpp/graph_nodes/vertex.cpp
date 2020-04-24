//
// Created by Michael Ong on 21/4/20.
//
#include "graph_nodes/vertex.hpp"

#include <utility>

using namespace swl;
using namespace std;

using ArgumentType  = rd::graph::node_argument_type;
using ArgumentScope = rd::graph::node_argument_scope;

rd::graph::nodes::vertex_effect_buffer::vertex_effect_buffer(
	vector<cx::point_3d> vtx,
	vector<cx::point_3d> off,
	vector<unsigned int> idx):
	node_effect   (0x20'00'01),
	vertices      (std::move(vtx)),
	vertex_offsets(std::move(off)),
	indices       (std::move(idx)) {
}

bool  rd::graph::nodes::vertex_effect_buffer::is_required() const {
	return true;
}


const rd::graph::node_argument
			rd::graph::nodes::vertex::OA_VertexBuffer   = rd::graph::node_argument{
	0x10'00'20, true, false, ArgumentType::typeDataBuffer, ArgumentScope::scopePerLayer };

const rd::graph::node_argument
			rd::graph::nodes::vertex::OA_VelocityBuffer = rd::graph::node_argument{
	0x10'00'21, false, false, ArgumentType::typeDataBuffer, ArgumentScope::scopePerLayer };


vector<rd::graph::node_argument>
			rd::graph::nodes::vertex::input_arguments         () const {
	return { };
}

vector<rd::graph::node_argument>
			rd::graph::nodes::vertex::output_arguments        () const {
	return { OA_VertexBuffer, OA_VelocityBuffer };
}


std::vector<cx::exp::ptr_ref<rd::graph::node_argument_input>>
			rd::graph::nodes::vertex::build_output_arguments  (const std::vector<cx::exp::ptr_ref<rd::entity>>& entities) const {

	cx::exp::ptr<node_argument_input, vertex_argument_input> output;

	unsigned long last_index{ 0 };
	output->entity_position.reserve(entities.size());

	for (const auto& entity: entities) {
		for(const auto& effect : entity->effects) {
			if (effect->id == 0x20'00'01) {
				last_index += output->entity_position.emplace_back(last_index);
				break;
			}
		}
	}

	// create vertex buffer
	// create index buffer

	return vector {
		cx::exp::ptr_ref<rd::graph::node_argument_input>{ output.abstract_pointer() },  /* arg_1: vertex buffer */
		cx::exp::ptr_ref<rd::graph::node_argument_input>{ nullptr }                     /* arg_2: velocity buffer */
	};
}

bool  rd::graph::nodes::vertex::shall_upload_resources  (const vector<cx::exp::ptr_ref<node_argument_input>>& inputs) const {
	return false;
}

void  rd::graph::nodes::vertex::upload_resources        (const vector<cx::exp::ptr_ref<node_argument_input>>& inputs) {
}


bool  rd::graph::nodes::vertex::shall_record_commands   (const vector<cx::exp::ptr_ref<node_argument_input>>& inputs) const {
	return false;
}

void  rd::graph::nodes::vertex::record_commands         (const vector<cx::exp::ptr_ref<node_argument_input>>& inputs) {
}
