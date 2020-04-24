//
// Created by Michael Ong on 21/4/20.
//
#pragma once

#include "header.hpp"
#include "point.hpp"

SNOW_OWL_NAMESPACE(cx)

template<typename Type, unsigned int Dimension>
struct matrix {

	std::array<vector<Type, Dimension>, Dimension> data;

	explicit matrix() = default;

	explicit matrix(const vector<Type, Dimension> &diagonal);
};

template<typename Type, unsigned int Dimension>
matrix<Type, Dimension>::matrix(const vector<Type, Dimension> &diagonal) {
	for (unsigned int I = 0; I < Dimension; I += 1) {
		vector<Type, Dimension> row;
		row.components[I] = diagonal.components[I];

		data[I] = row;
	}
}

SNOW_OWL_NAMESPACE_END
