//
// Created by Michael Ong on 21/4/20.
//
#include <world_renderer.hpp>
#include <utility>

using namespace swl;
using namespace std;


rd::world_renderer::world_renderer(string name): name(std::move(name)) {

}

rd::world_renderer::world_renderer(world_renderer &&mov) noexcept:
	name  (std::move(mov.name )),
	nodes (std::move(mov.nodes)),
	tree  (std::move(mov.tree )) {


}


rd::world_renderer::_node_graph::_node_graph(
	cx::driver_handle                         node,
	std::vector<cx::exp::ptr<_node_graph>>   children):
	node(node), children(std::move(children)) { }

rd::world_renderer::_node_graph::_node_graph(world_renderer::_node_graph &&mov) noexcept:
	node(mov.node), children(std::move(mov.children)) { }


void  rd::world_renderer::compile           () {


}

std::vector<cx::exp::ptr_ref<rd::world_renderer::_node_graph>>
			rd::world_renderer::_node_graph::find_leaf() {

	if (children.empty()) {
		return { cx::exp::ptr_ref{ this } };
	}

	std::vector<cx::exp::ptr_ref<_node_graph>> fill{};

	for (cx::exp::ptr<_node_graph> &item : children) {
		auto result = item->find_leaf();
		fill.insert(fill.end(), result.begin(), result.end());
	}

	return fill;
}

cx::exp::ptr_ref<rd::world_renderer::_node_graph>
			rd::world_renderer::_node_graph::find(cx::driver_handle handle) {

	if (node == handle) {
		return cx::exp::ptr_ref<_node_graph>{this };
	}

	for(_node_graph &item : children) {
		auto result = item.find(handle);

		if(result) {
			return result;
		}
	}

	return cx::exp::ptr_ref<_node_graph>(nullptr);
}
