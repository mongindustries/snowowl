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

struct diffuse_effect_texture: node_effect {

	diffuse_effect_texture
				(std::filesystem::path path, const cx::rect& frame_rect, std::array<cx::point_2d, 4> tex_coord);

	[[nodiscard]] bool
				is_required() const override;

	std::filesystem::path       texture_path;

	cx::rect                    frame_rect;

	std::array<cx::point_2d, 4> tex_coord;
};

struct diffuse: node {

	static const node_argument IA_VertexBuffer;

	static const node_argument OA_DiffuseBuffer;


	[[nodiscard]]
	std::vector<node_argument>
				input_arguments   () const override;

	[[nodiscard]]
	std::vector<node_argument>
				output_arguments  () const override;
};

SNOW_OWL_NAMESPACE_END
