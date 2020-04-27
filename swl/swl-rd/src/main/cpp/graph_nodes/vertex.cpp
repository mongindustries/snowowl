//
// Created by Michael Ong on 21/4/20.
//
#include "graph_nodes/vertex.hpp"

#include <utility>

using namespace swl;
using namespace std;

SNOW_OWL_NAMESPACE(rd::graph)

using ArgumentType  = node_argument_type;
using ArgumentScope = node_argument_scope;

nodes::vertex_effect_buffer::vertex_effect_buffer
	(vector<cx::point_3d> vtx, vector<cx::point_3d> off, vector<unsigned int> idx):
	node_effect   (0x20'00'01),

	vertices      (std::move(vtx)),
	vertex_offsets(std::move(off)),
	indices       (std::move(idx)) {
}

bool  nodes::vertex_effect_buffer::is_required() const {
	return true;
}


const node_argument
nodes::vertex::OA_VertexBuffer = node_argument{
			0x10'00'20, true, false, ArgumentType::typeDataBuffer, ArgumentScope::scopePerLayer };

const node_argument
			nodes::vertex::OA_VelocityBuffer = node_argument{
	0x10'00'21, false, false, ArgumentType::typeDataBuffer, ArgumentScope::scopePerLayer };

const node_argument
			nodes::vertex::OA_EntityOffset   = node_argument{
	0x10'00'21, true, false, ArgumentType::typeDataBuffer, ArgumentScope::scopePerLayer };


nodes::vertex_argument_state::vertex_argument_state  ():
			buffer       (nullptr),
			index_buffer (nullptr) { }

nodes::vertex_argument_state::vertex_argument_state  (vertex_argument_state&& mov) noexcept:
			buffer       (std::move(mov.buffer)),
			index_buffer (std::move(mov.index_buffer)) { }

nodes::vertex_argument_state::~vertex_argument_state () = default;


nodes::vertex_argument_state&
			nodes::vertex_argument_state::operator=  (vertex_argument_state&& mov) noexcept {

	return *this;
}

vector<node_argument>
			nodes::vertex::input_arguments           () const {
	return { };
}

vector<node_argument>
			nodes::vertex::output_arguments          () const {
	return { OA_VertexBuffer, OA_VelocityBuffer, OA_EntityOffset };
}


std::vector<cx::driver_handle>
			nodes::vertex::effects                   () const {

	return { 0x20'00'01 };
}


std::vector<cx::exp::ptr<node_argument_state>>
			nodes::vertex::create_resource           () const {

	cx::exp::ptr<node_argument_state, vertex_argument_state>  vertex_data{ nullptr }; {
		// create vertex buffer
		// create index buffer
	}

	cx::exp::ptr<node_argument_state, vertex_entity_state>    entity_data{ nullptr }; {
		// create entity offset
	}

	cx::exp::ptr<node_argument_state, vertex_velocity_state>  velocity_data{ nullptr }; {
	}

	vector<cx::exp::ptr<node_argument_state>> resources;

	resources.emplace_back(vertex_data    .abstract_and_release()); /* arg_1: vertex buffer */
	resources.emplace_back(velocity_data  .abstract_and_release()); /* arg_2: velocity data*/
	resources.emplace_back(entity_data    .abstract_and_release()); /* arg_3: entity data */
	
	return resources;
}

void  nodes::vertex::ingest_data               (const node_context& context) {

}

SNOW_OWL_NAMESPACE_END
