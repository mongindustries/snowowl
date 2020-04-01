//
// Created by Michael Ong on 1/4/20.
//
#pragma once

#include "headerconv.hpp"

#include <cstdio>

SNOW_OWL_NAMESPACE(cx)

typedef char* DataPointer;

typedef char const* DataConstPointer;

struct Data {

	Data(const DataPointer contents, std::size_t size);


	DataConstPointer data() const noexcept { return contents; }

	std::size_t size() const noexcept { return contentSize; }

protected:

	DataPointer contents;

	std::size_t contentSize;
};

struct MutableData: Data {

	void set(DataPointer newContents);

	void modify();
};

SNOW_OWL_NAMESPACE_END
