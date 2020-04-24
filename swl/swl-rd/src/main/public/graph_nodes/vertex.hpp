//
// Created by Michael Ong on 21/4/20.
//

#include <vector>
#include <filesystem>

#include <header.hpp>
#include <point.hpp>

#include "graph_node.hpp"
#include "graph_node_effect.hpp"

SNOW_OWL_NAMESPACE(rd::graph::nodes)

struct vertex_effect_buffer: public node_effect {

	vertex_effect_buffer  (
		std::vector<cx::point_3d> vtx,
		std::vector<cx::point_3d> off,
		std::vector<unsigned int> idx
	);

	[[nodiscard]]
	bool  is_required   () const override;

	std::vector<cx::point_3d> vertices;
	std::vector<cx::point_3d> vertex_offsets;

	std::vector<unsigned int> indices;
};

struct vertex_argument_input: public node_argument_input {

	// vk::UniqueBuffer vertex_buffer;
	// vk::UniqueBuffer index_buffer;

	std::vector<unsigned long> entity_position;
};

struct vertex: public node {

	static const node_argument OA_VertexBuffer;

	static const node_argument OA_VelocityBuffer;


	explicit vertex               () = default;


	[[nodiscard]]
	std::vector<node_argument>
				input_arguments         () const override;

	[[nodiscard]]
	std::vector<node_argument>
				output_arguments        () const override;


	[[nodiscard]]
	std::vector<cx::exp::ptr_ref<node_argument_input>>
				build_output_arguments  (const std::vector<cx::exp::ptr_ref<entity>>& entities) const override;


	[[nodiscard]]
	bool  shall_upload_resources  (const std::vector<cx::exp::ptr_ref<node_argument_input>> &inputs) const override;

	void  upload_resources        (const std::vector<cx::exp::ptr_ref<node_argument_input>> &inputs) override;


	[[nodiscard]]
	bool  shall_record_commands   (const std::vector<cx::exp::ptr_ref<node_argument_input>> &inputs) const override;

	void  record_commands         (const std::vector<cx::exp::ptr_ref<node_argument_input>> &inputs) override;
};

SNOW_OWL_NAMESPACE_END
