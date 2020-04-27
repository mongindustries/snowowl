//
// Created by Michael Ong on 21/4/20.
//
#pragma once

#include <vector>

#include <header.hpp>
#include <point.hpp>

#include "graph_node.hpp"
#include "graph_node_effect.hpp"

#include "graphics_buffer.hpp"
#include "graphics_shader.hpp"

SNOW_OWL_NAMESPACE(rd::graph::nodes)

struct vertex_effect_buffer final : node_effect {

	vertex_effect_buffer
				(std::vector<cx::point_3d> vtx, std::vector<cx::point_3d> off, std::vector<unsigned int> idx);

	[[nodiscard]] bool
				is_required() const override;

	std::vector<cx::point_3d> vertices        {};
	std::vector<cx::point_3d> vertex_offsets  {};

	std::vector<unsigned int> indices         {};
};

struct vertex_argument_state: node_argument_state {
		
	vertex_argument_state             (const vertex_argument_state&) = delete;

	vertex_argument_state& operator=  (const vertex_argument_state&) = delete;

	
	vertex_argument_state             (vertex_argument_state&& mov) noexcept;

	vertex_argument_state& operator=  (vertex_argument_state&& mov) noexcept;


	vertex_argument_state   ();

	~vertex_argument_state  ();


	cx::exp::ptr<gx::graphics_buffer<gx::graphics_buffer_type::typeData>> buffer;
	cx::exp::ptr<gx::graphics_buffer<gx::graphics_buffer_type::typeData>> index_buffer;

	gx::graphics_shader                                                   shader;
};

struct vertex_entity_state: node_argument_state {
	std::vector<unsigned long> offsets{};
};

struct vertex_velocity_state: node_argument_state {
};

struct vertex final : node {

	/// Output argument: Represents the vertex buffer, index buffer, and the shader this node defines.
	static const node_argument OA_VertexBuffer;

	/// Output argument: Represents the delta of each vertex entry in the vertex buffer.
	static const node_argument OA_VelocityBuffer;

	/// Output argument: Represents a layer's entity offset on the vertex buffer's <code>index_buffer</code> output argument.
	static const node_argument OA_EntityOffset;


	explicit vertex         () = default;


	[[nodiscard]] std::vector<node_argument>
				input_arguments   () const override;

	[[nodiscard]] std::vector<node_argument>
				output_arguments  () const override;


	[[nodiscard]] std::vector<cx::driver_handle>
				effects           () const override;



	[[nodiscard]] std::vector<cx::exp::ptr<node_argument_state>>
				create_resource   () const override;

	void  ingest_data       (const node_context &context) override;
};

SNOW_OWL_NAMESPACE_END
