//
// Created by Michael Ong on 21/4/20.
//
#pragma once

#include <vector>
#include <filesystem>

#include <header.hpp>
#include <rect.hpp>

#include "graph_node.hpp"
#include "graph_node_effect.hpp"

SNOW_OWL_NAMESPACE(rd::graph::nodes)

struct diffuse_effect_texture: public node_effect {

	std::filesystem::path       texturePath;

	cx::rect                    frameRect;

	std::array<cx::point_2d, 4> texCoords;

	// tint


	diffuse_effect_texture(
		std::filesystem::path       path,
		const cx::rect&             frameRect,
		std::array<cx::point_2d, 4> texCoords);


	[[nodiscard]]
	bool is_required() const override;
};

struct diffuse: public node {

	static const node_argument IA_VertexBuffer;

	static const node_argument OA_DiffuseBuffer;


	[[nodiscard]]
	std::vector<node_argument>
				input_arguments            () const override;

	[[nodiscard]]
	std::vector<node_argument>
				output_arguments           () const override;
};

SNOW_OWL_NAMESPACE_END
