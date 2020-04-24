//
// Created by Michael Ong on 21/4/20.
//
#include <entity.hpp>
#include <utility>

using namespace std;
using namespace swl;

SNOW_OWL_NAMESPACE(rd)

entity::entity
			():
			transform() { }

entity::entity
			(std::string name):
			name(std::move(name)), transform({ 1, 1, 1, 1 }) { }

entity::entity
			(rd::entity &&mov) noexcept:
			name(std::move(mov.name)), transform(mov.transform), effects(std::move(mov.effects)) { }

entity& entity::operator=  (entity&& mov) noexcept {

	name      = std::move(mov.name);
	transform = mov.transform;

	effects   = std::move(mov.effects);

	return *this;
}

void   entity::update      (chrono::milliseconds offset) { }

SNOW_OWL_NAMESPACE_END
