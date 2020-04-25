//
// Created by Michael Ong on 21/4/20.
//
#pragma once

#include <vector>

#include <header.hpp>
#include <point.hpp>

#include <vulkan/vulkan_import.h>

#include "graph_node.hpp"
#include "graph_node_effect.hpp"

SNOW_OWL_NAMESPACE(rd::graph::nodes)

struct vertex_effect_buffer final : node_effect {

	vertex_effect_buffer
				(std::vector<cx::point_3d> vtx, std::vector<cx::point_3d> off, std::vector<unsigned int> idx);

	[[nodiscard]] bool
				is_required() const override;

	std::vector<cx::point_3d> vertices;
	std::vector<cx::point_3d> vertex_offsets;

	std::vector<unsigned int> indices;
};

struct vertex_argument_input: node_argument_input {
		
	vertex_argument_input             (const vertex_argument_input&) = delete;

	vertex_argument_input& operator=  (const vertex_argument_input&) = delete;

	
	vertex_argument_input             (vertex_argument_input&& mov) noexcept;

	vertex_argument_input& operator=  (vertex_argument_input&& mov) noexcept;


	vertex_argument_input   ();

	~vertex_argument_input  ();


	vk::UniqueBuffer vertex_buffer;
	vk::UniqueBuffer index_buffer;
};

struct vertex final : node {

	static const node_argument OA_VertexBuffer;

	static const node_argument OA_VelocityBuffer;


	explicit vertex         () = default;


	[[nodiscard]] std::vector<node_argument>
				input_arguments   () const override;

	[[nodiscard]] std::vector<node_argument>
				output_arguments  () const override;


	[[nodiscard]] std::vector<cx::driver_handle>
				effects           () const override;



	[[nodiscard]] std::vector<cx::exp::ptr<node_argument_input>>
				create_resource   () const override;

	void  ingest_data       (const node_context &context) override;
};

SNOW_OWL_NAMESPACE_END
