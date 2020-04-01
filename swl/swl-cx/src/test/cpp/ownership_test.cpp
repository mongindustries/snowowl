//
// Created by Michael Ong on 1/4/20.
//

#include "ownership.hpp"
#include <iostream>
#include <vector>

using namespace std;
using namespace swl::cx;

struct anotherStruct {

	int boop;

	void foo() {

	}
};

struct somethingStruct {

	int baz;
	std::string bleep;

	Own<anotherStruct> refer;

	void foo() {

	}

	void bar() const {

	}
};

void intentionToModify(MutateOwn<somethingStruct> mutate) {

	mutate = [](somethingStruct* const item) {
		item->refer = [](anotherStruct* const item) {
			item->foo();
		};
	};
}

void intentionToRead(Lend<somethingStruct> lend) {

	const auto something = lend.refer->boop;
}

void something() {

	Own<somethingStruct> owner;

	intentionToModify(owner);
	intentionToRead(*owner);
}


struct Node {
	LateLend<Node> _parent;
	std::vector<Own<Node>> _children;
};

void modify(Lend<Node> node) {

}

void root() {

	auto root = Own<Node>({ nullptr, { } });

	root = [&root](Node *const mut) {

		mut->_children.push_back({ { root, { } } });
		mut->_children.push_back({ { root, { } } });
		mut->_children.push_back({ { root, { } } });
		mut->_children.push_back({ { root, { } } });

		mut->_children.push_back({ { root, { } } });
	};
}
