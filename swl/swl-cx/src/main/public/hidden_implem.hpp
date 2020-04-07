//
// Created by Michael Ong on 31/3/20.
//
#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdelete-incomplete"

#include "headerconv.hpp"

SNOW_OWL_NAMESPACE(cx)

template<typename Wrapped>
struct PartialImplementation {

	PartialImplementation(): _implementation(nullptr) { }

	explicit PartialImplementation(Wrapped* introduce): _implementation(introduce) { }


	PartialImplementation(PartialImplementation<Wrapped> &&mov): _implementation(mov._implementation) {
		mov._implementation = nullptr;
	}

	PartialImplementation<Wrapped>& operator=(PartialImplementation<Wrapped> &&mov) {
		_implementation = mov._implementation;
		mov._implementation = nullptr;
	}


	PartialImplementation(const PartialImplementation<Wrapped>&) = delete;

	PartialImplementation<Wrapped>& operator=(const PartialImplementation<Wrapped>&) = delete;


	~PartialImplementation() {
		if (_implementation != nullptr) {
			delete _implementation;
		}
	}


	Wrapped& operator()() const {
		return *_implementation;
	}

	void invalidate() {
		_implementation = nullptr;
	}

private:

	Wrapped* _implementation;
};

SNOW_OWL_NAMESPACE_END

#pragma GCC diagnostic pop
