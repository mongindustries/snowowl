//
// Created by Michael Ong on 26/3/20.
//
#pragma once

#include "header.hpp"

#include <array>
#include <cstdint>
#include <immintrin.h>

SNOW_OWL_NAMESPACE(cx)

template<typename ComponentType, uint16_t Components>
struct vector {

	vector          () = default;

	explicit vector (const std::array<ComponentType, Components> &values) noexcept: components(values) { }


	[[nodiscard]] constexpr uint16_t
					size    () const { return Components; }

	[[nodiscard]] constexpr ComponentType const*
					data    () const { return components.data(); }


	std::array<ComponentType, Components> components;
};


template<typename ComponentType>
struct vector<ComponentType, 2> {

	vector          () = default;

	vector          (ComponentType _1, ComponentType _2): components({_1, _2 }) { }

	explicit vector (long values) noexcept: components(values) { }


	[[nodiscard]] constexpr uint16_t
					size    () const { return 2; }

	[[nodiscard]] constexpr ComponentType const*
					data    () const { return components.data(); }


	[[nodiscard]] ComponentType
					x       () const { return components[0]; }

	[[nodiscard]] ComponentType
					y       () const { return components[1]; }


	std::array<ComponentType, 2> components;
};

template<typename ComponentType>
struct vector<ComponentType, 3> {

	vector          () = default;

	vector          (
		ComponentType _1,
		ComponentType _2,
		ComponentType _3): components({_1, _2, _3 }) { }

	explicit vector (const std::array<ComponentType, 3> &values) noexcept: components(values) { }


	[[nodiscard]] constexpr uint16_t
				size      () const { return 3; }

	[[nodiscard]] constexpr ComponentType const*
				data      () const { return components.data(); }


	[[nodiscard]] float
				x         () const { return components[0]; }

	[[nodiscard]] float
				y         () const { return components[1]; }

	[[nodiscard]] float
				z         () const { return components[2]; }


	std::array<ComponentType, 3> components;
};

template<typename ComponentType>
struct vector<ComponentType, 4> {

	vector          () = default;

	vector          (
		ComponentType _1,
		ComponentType _2,
		ComponentType _3,
		ComponentType _4): components({_1, _2, _3, _4 }) { }

	explicit vector (const std::array<ComponentType, 4> &values) noexcept: components(values) { }

	[[nodiscard]] constexpr uint16_t
				size      () const { return 4; }

	[[nodiscard]] constexpr ComponentType const*
				data      () const { return components.data(); }


	[[nodiscard]] float
				x         () const { return components[0]; }

	[[nodiscard]] float
				y         () const { return components[1]; }

	[[nodiscard]] float
				z         () const { return components[2]; }

	[[nodiscard]] float
				w         () const { return components[3]; }


	std::array<ComponentType, 4> components;
};


typedef vector<float, 2> point_2d;

typedef vector<float, 3> point_3d;


typedef vector<int, 2> size_2d;

typedef vector<int, 3> size_3d;

SNOW_OWL_NAMESPACE_END
