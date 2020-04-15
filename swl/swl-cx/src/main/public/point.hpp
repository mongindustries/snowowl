//
// Created by Michael Ong on 26/3/20.
//
#pragma once

#include "header.hpp"

#include <array>
#include <immintrin.h>

SNOW_OWL_NAMESPACE(cx)

template<typename ComponentType, uint16_t Components>
struct Vector {

	std::array<ComponentType, Components> components;

	Vector() = default;

	explicit Vector(const std::array<ComponentType, Components> &values) noexcept: components(values) { }

	[[nodiscard]] constexpr uint16_t size() const { return Components; }

	[[nodiscard]] constexpr ComponentType const* data() const { return components.data(); }
};

template<uint16_t Index, typename ComponentType, uint16_t Components>
ComponentType component(const Vector<ComponentType, Components> &vector) {
	return vector.components[Index];
}


template<typename ComponentType>
struct Vector<ComponentType, 2> {

	std::array<ComponentType, 2> components;

	Vector() = default;

	explicit Vector(const std::array<ComponentType, 2> &values) noexcept: components(values) { }

	Vector(ComponentType _1, ComponentType _2): components({ _1, _2 }) { }

	[[nodiscard]] constexpr uint16_t size() const { return 2; }

	[[nodiscard]] constexpr ComponentType const* data() const { return components.data(); }


	[[nodiscard]] ComponentType x() const { return component<0>(*this); }

	[[nodiscard]] ComponentType y() const { return component<1>(*this); }
};

template<typename ComponentType>
struct Vector<ComponentType, 3> {

	std::array<ComponentType, 3> components;

	Vector() = default;

	explicit Vector(const std::array<ComponentType, 3> &values) noexcept: components(values) { }

	Vector(ComponentType _1, ComponentType _2, ComponentType _3): components({ _1, _2, _3 }) { }

	[[nodiscard]] constexpr uint16_t size() const { return 3; }

	[[nodiscard]] constexpr ComponentType const* data() const { return components.data(); }


	[[nodiscard]] float x() const { return component<0>(*this); }

	[[nodiscard]] float y() const { return component<1>(*this); }

	[[nodiscard]] float z() const { return component<2>(*this); }
};

template<typename ComponentType>
struct Vector<ComponentType, 4> {

	std::array<ComponentType, 4> components;

	Vector() = default;

	explicit Vector(const std::array<ComponentType, 4> &values) noexcept: components(values) { }

	Vector(ComponentType _1, ComponentType _2, ComponentType _3, ComponentType _4): components({ _1, _2, _3, _4 }) { }

	[[nodiscard]] constexpr uint16_t size() const { return 4; }

	[[nodiscard]] constexpr ComponentType const* data() const { return components.data(); }


	[[nodiscard]] float x() const { return component<0>(*this); }

	[[nodiscard]] float y() const { return component<1>(*this); }

	[[nodiscard]] float z() const { return component<2>(*this); }

	[[nodiscard]] float w() const { return component<3>(*this); }
};


typedef Vector<float, 2> Point2D;

typedef Vector<float, 3> Point3D;


typedef Vector<int, 2> Size2D;

typedef Vector<int, 3> Size3D;

SNOW_OWL_NAMESPACE_END
