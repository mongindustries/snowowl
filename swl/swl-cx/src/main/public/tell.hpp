//
// Created by Michael Ong on 4/4/20.
//
#pragma once

#include <functional>
#include "header.hpp"

SNOW_OWL_NAMESPACE(cx)

template<typename Object>
Object* tell(Object *instance, std::function<void(Object *)> block) {
	block(instance);
	return instance;
}

template<typename Object>
Object tell(Object instance, std::function<void(Object&)> block) {
	block(instance);
	return instance;
}

#define TELL_O(Type, Block) cx::tell<Type>({},          [](Type &object) Block)
#define TELL_P(Type, Block) cx::tell<Type>(new Type(),  [](Type *object) Block)

SNOW_OWL_NAMESPACE_END
