//
// Created by Michael Ong on 22/4/20.
//
#include <entity.hpp>
#include <layer.hpp>

#include <world.hpp>
#include <world_renderer.hpp>

#include <graph_nodes/diffuse.hpp>
#include <graph_nodes/vertex.hpp>

#include <utility>
#include <filesystem>

#include <gtest/gtest.h>

using namespace swl;
using namespace std;

class x_entity: public rd::entity {
public:
	explicit x_entity(const std::string &string) : entity(string) {
	}
};

TEST(renderer, ShouldConstruct) {

	namespace nodes = rd::graph::nodes;

	rd::world_renderer graph{ "default" };

	auto nodeid_vertex  = graph.add_node(rd::graph::nodes::vertex ());
	auto nodeid_diffuse = graph.add_node(rd::graph::nodes::diffuse());

	std::vector<pair<rd::graph::node_argument, rd::graph::node_argument>> linkages {
		{ nodes::vertex  ::OA_VertexBuffer, nodes::diffuse ::IA_VertexBuffer }
	};

	graph.set_root(nodeid_vertex);
	graph.add_link(nodeid_vertex, nodeid_diffuse, linkages);

	graph.set_output(nodeid_diffuse, nodes::diffuse::OA_DiffuseBuffer);

	rd::layer layer{"base", rd::layer_type::typeOnScene }; {

		rd::entity testEntity{"test_entity" }; {
			testEntity.mark_effect(nodes::vertex_effect_buffer   {
				{}, {}, {} });
			testEntity.mark_effect(nodes::diffuse_effect_texture {
				filesystem::path{"texture"} / "test.tex",
				cx::rect{ {0, 0 }, { 100, 100 } },
				{} });
		} layer.add_entity(testEntity);

		testEntity.name = "sets_entity"; {
			testEntity.mark_effect(nodes::vertex_effect_buffer   {
				{}, {}, {} });
			testEntity.mark_effect(nodes::diffuse_effect_texture {
				filesystem::path{"texture"} / "sets.tex",
				cx::rect{ {0, 0 }, { 100, 100 } },
				{} });
		} layer.add_entity(testEntity);

		layer.add_entity_static(x_entity("another test"));
	}

	rd::world world; {
		auto layer_ref  = world.add_layer(layer);

		auto handle = world.add_renderer(::move(graph));
		if (layer_ref.reference) {
			layer_ref.reference->bind_renderer(handle);
		}
	}

	std::cout << "Done" << std::endl;
}

int main() {

	testing::InitGoogleTest();

	return RUN_ALL_TESTS();
}
