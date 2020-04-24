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

SNOW_OWL_NAMESPACE_END
