//
// Created by Michael Ong on 21/4/20.
//
#include <entity.hpp>
#include <utility>

using namespace std;
using namespace swl;

rd::entity::entity(std::string name):
	name      (std::move(name)),
	transform ({ 1, 1, 1, 1 }) {

}

rd::entity::entity(rd::entity &&mov) noexcept:
	name      (std::move(mov.name)),
	transform (mov.transform),
	effects   (std::move(mov.effects)) {

	mov.effects.clear();
}


void rd::entity::update(
	chrono::milliseconds offset) {

}
