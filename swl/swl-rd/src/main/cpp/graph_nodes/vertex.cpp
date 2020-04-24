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
			nodes::vertex::OA_VertexBuffer   = node_argument{
	0x10'00'20, true, false, ArgumentType::typeDataBuffer, ArgumentScope::scopePerLayer };

const node_argument
			nodes::vertex::OA_VelocityBuffer = node_argument{
	0x10'00'21, false, false, ArgumentType::typeDataBuffer, ArgumentScope::scopePerLayer };


nodes::vertex_argument_input::vertex_argument_input
	() {}

nodes::vertex_argument_input::vertex_argument_input
	(vertex_argument_input&& mov) noexcept:
	vertex_buffer  (::move(mov.vertex_buffer)),
	index_buffer   (::move(mov.index_buffer)) {
	
}

nodes::vertex_argument_input::~vertex_argument_input
	() {}

nodes::vertex_argument_input&
			nodes::vertex_argument_input::operator=(vertex_argument_input&& mov) noexcept {

	vertex_buffer = std::move(mov.vertex_buffer);
	index_buffer  = std::move(mov.index_buffer);

	return *this;
}

vector<node_argument>
			nodes::vertex::input_arguments   () const {
	return { };
}

vector<node_argument>
			nodes::vertex::output_arguments  () const {
	return { OA_VertexBuffer, OA_VelocityBuffer };
}


std::vector<cx::driver_handle>
			nodes::vertex::effects           () const {

	return { 0x20'00'01 };
}


std::vector<cx::exp::ptr<node_argument_input>>
			nodes::vertex::create_resource   () const {

	cx::exp::ptr<node_argument_input, vertex_argument_input> vertex_data{ nullptr };

	// create vertex buffer
	// create index buffer

	vector<cx::exp::ptr<node_argument_input>> resources;

	resources.emplace_back(vertex_data.abstract()); /* arg_1: vertex buffer */
	resources.emplace_back(nullptr);
	
	return resources;
}

void  nodes::vertex::ingest_data       (const node_context& context) {
}

SNOW_OWL_NAMESPACE_END
