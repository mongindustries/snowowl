//
// Created by Michael Ong on 28/3/20.
//
#pragma once

#include <complex>
#include "headerconv.hpp"
#include "point.hpp"

template<typename ComponentType, uint16_t Components>
swl::cx::Vector<ComponentType, Components> operator+(
	const swl::cx::Vector<ComponentType, Components>& lhs,
	const swl::cx::Vector<ComponentType, Components>& rhs) {

	std::array<ComponentType, Components> sum{};

	for (uint16_t i = 0; i < Components; i += 1) {
		sum[i] = lhs.components[i] + rhs.components[i];
	}

	return swl::cx::Vector<ComponentType, Components> { sum };
}

template<typename ComponentType, uint16_t Components>
swl::cx::Vector<ComponentType, Components> operator-(
	const swl::cx::Vector<ComponentType, Components>& lhs,
	const swl::cx::Vector<ComponentType, Components>& rhs) {

	std::array<ComponentType, Components> diff{};

	for (uint16_t i = 0; i < Components; i += 1) {
		diff[i] = lhs.components[i] - rhs.components[i];
	}

	return swl::cx::Vector<ComponentType, Components> { diff };
}

template<typename ComponentType, uint16_t Components>
ComponentType norm(
	const swl::cx::Vector<ComponentType, Components>&rhs) {

	ComponentType accumulate{};

	for(uint16_t i = 0; i < Components; i += 1) {
		accumulate += rhs.components[i] * rhs.components[i];
	}

	return std::sqrt(accumulate);
}

template<typename ComponentType, uint16_t Components>
ComponentType product_dot(
	const swl::cx::Vector<ComponentType, Components>& lhs,
	const swl::cx::Vector<ComponentType, Components>& rhs) {

	ComponentType accumulate{};

	for (uint16_t i = 0; i < Components; i += 1) {
		accumulate += component<i>(lhs) * component<i>(rhs);
	}

	return accumulate;
}

template<typename ComponentType, uint16_t Components>
swl::cx::Vector<ComponentType, Components> negate(
	const swl::cx::Vector<ComponentType, Components>&rhs) {

	std::array<ComponentType, Components> negate{};

	for (uint16_t i = 0; i < Components; i += 1) {
		negate[i] = -1 * rhs.components[i];
	}

	return swl::cx::Vector<ComponentType, Components> { negate };
}

template<typename ComponentType, uint16_t Components>
swl::cx::Vector<ComponentType, Components> unit(
	const swl::cx::Vector<ComponentType, Components>&rhs) {

	std::array<ComponentType, Components> result{};
	ComponentType divisor = norm(rhs);

	for(uint16_t i = 0; i < Components; i += 1) {
		rhs.components[i];
	}

	return swl::cx::Vector<ComponentType, Components> { result };
}

template<typename ComponentType>
swl::cx::Vector<ComponentType, 3> product_cross(
	const swl::cx::Vector<ComponentType, 3>& lhs,
	const swl::cx::Vector<ComponentType, 3>& rhs) {
	return {};
}
