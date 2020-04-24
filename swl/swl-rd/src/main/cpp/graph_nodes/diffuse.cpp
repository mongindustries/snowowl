//
// Created by Michael Ong on 21/4/20.
//
#include "graph_nodes/diffuse.hpp"

#include <utility>

using namespace std;
using namespace swl;

using ArgumentType  = rd::graph::node_argument_type;
using ArgumentScope = rd::graph::node_argument_scope;

rd::graph::nodes::diffuse_effect_texture::diffuse_effect_texture(
	filesystem::path path,
	const cx::rect &frameRect,
	std::array<cx::point_2d, 4> texCoords):
	node_effect(0x30'00'01),
	texturePath(std::move(path)),
	frameRect(frameRect),
	texCoords(texCoords) {

}

bool  rd::graph::nodes::diffuse_effect_texture::is_required() const {
	return true;
}


const rd::graph::node_argument
			rd::graph::nodes::diffuse::IA_VertexBuffer = rd::graph::node_argument{
	0x20'00'10, true, false, ArgumentType::typeDataBuffer, ArgumentScope::scopePerLayer };

const rd::graph::node_argument
			rd::graph::nodes::diffuse::OA_DiffuseBuffer = rd::graph::node_argument{
	0x20'00'20, true, true, ArgumentType::typeTexture2D, ArgumentScope::scopePerLayer };


vector<rd::graph::node_argument>
			rd::graph::nodes::diffuse::input_arguments() const {
	return { IA_VertexBuffer };
}

vector<rd::graph::node_argument>
			rd::graph::nodes::diffuse::output_arguments() const {
	return { OA_DiffuseBuffer };
}
