//
// Created by Michael Ong on 31/3/20.
//
#pragma once

#include "headerconv.hpp"

SNOW_OWL_NAMESPACE(cx)

template<typename Wrapped>
struct Himplem {

	Himplem(): __implem(new Wrapped()) { }

	Himplem(Wrapped* introduce): __implem(introduce) { }

	~Himplem() {
		if (__implem != nullptr) {
		delete __implem;
		}
	}


	Wrapped& operator()() const {
		return *__implem;
	}

	void invalidate() {
		__implem = nullptr;
	}

private:

	Wrapped* __implem;
};

SNOW_OWL_NAMESPACE_END
