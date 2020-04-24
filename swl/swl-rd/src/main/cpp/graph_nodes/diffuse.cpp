//
// Created by Michael Ong on 21/4/20.
//
#include "graph_nodes/diffuse.hpp"

#include <utility>

using namespace std;
using namespace swl;

SNOW_OWL_NAMESPACE(rd::graph)

using ArgumentType  = node_argument_type;
using ArgumentScope = node_argument_scope;

nodes::diffuse_effect_texture::diffuse_effect_texture
	(filesystem::path path, const cx::rect &frame_rect, std::array<cx::point_2d, 4> tex_coord):
	node_effect(0x30'00'01), texture_path(std::move(path)), frame_rect(frame_rect), tex_coord(tex_coord) {

}

bool  nodes::diffuse_effect_texture::is_required() const {
	return true;
}


const node_argument
			nodes::diffuse::IA_VertexBuffer   = node_argument{
	0x20'00'10, true, false, ArgumentType::typeDataBuffer, ArgumentScope::scopePerLayer };

const node_argument
			nodes::diffuse::OA_DiffuseBuffer  = node_argument{
	0x20'00'20, true, true, ArgumentType::typeTexture2D, ArgumentScope::scopePerLayer };


vector<node_argument>
			nodes::diffuse::input_arguments   () const {
	return { IA_VertexBuffer };
}

vector<node_argument>
			nodes::diffuse::output_arguments  () const {
	return { OA_DiffuseBuffer };
}

SNOW_OWL_NAMESPACE_END
