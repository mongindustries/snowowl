//
// Created by Michael Ong on 31/3/20.
//
#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdelete-incomplete"

#include "headerconv.hpp"

SNOW_OWL_NAMESPACE(cx)

template<typename Wrapped>
struct Himplem {

	Himplem(): _implem(new Wrapped()) { }

	explicit Himplem(Wrapped* introduce): _implem(introduce) { }


	~Himplem() {
		if (_implem != nullptr) {
		delete _implem;
		}
	}


	Wrapped& operator()() const {
		return *_implem;
	}

	void invalidate() {
		_implem = nullptr;
	}

private:

	Wrapped* _implem;
};

SNOW_OWL_NAMESPACE_END

#pragma GCC diagnostic pop
