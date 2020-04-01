//
// Created by Michael Ong on 1/4/20.
//
#pragma once

#include "headerconv.hpp"

#include <utility>
#include <functional>

SNOW_OWL_NAMESPACE(cx)

struct BadOwnership: std::exception { };

template<typename Type>
struct Own;

/// Definite ownership mutating consumer.
template<typename Type>
using MutateOwn = Own<Type>&;

/// Definite upon-instantiation borrowed ownership.
template<typename Type>
using Lend = Type const&;

/**
 * An object that holds ownership over a pointer.
 *
 * This is similar to <code>std's</code> <code>unique_ptr</code> construct wherein
 * <code>Own</code>'s ownership can only be moved.
 *
 * But it is different from <code>unique_ptr</code> as it can be <code>Lend</code>ed
 * at any time.
 *
 * @tparam Type The type that <code>Own</code> holds.
 */
template<typename Type>
struct Own {

	template<typename... Params>
	Own(Params... params): value(new Type(params...)) { }

	template<typename = std::enable_if<std::is_move_constructible_v<Type>>>
	Own(const Type &&move): value(new Type(std::move(move))) { }

	template<typename = std::enable_if<std::is_copy_constructible_v<Type>>>
	Own(const Type &copy): value(new (Type)(copy)) { }



	Own(const Own &&cpy) = delete; // own cannot be copied, selfish siya eh

	Own(Own &&mov) = default; // but it can be moved

	~Own() {
		if (value != nullptr) {
			delete value;
		}
	}

	bool isValid() const {
		return value != nullptr;
	}


	Type const* operator->() const {

		if (!isValid()) {
			throw BadOwnership();
		}

		return value;
	}

	Lend<Type> operator* () const {

		if (!isValid()) {
			throw BadOwnership();
		}

		return *value;
	}


	void mutate(std::function<void(Type* const)> mutate) {
		mutate(value);
	}

	Own<Type> operator= (Own<Type> &copy) = delete;

	void operator= (std::function<void(Type* const)> rhs) {
		mutate(rhs);
	}


private:

	Type *value;
};

/// Late binding borrowed ownership. Use <code>bind()</code> to indicate lending.
template<typename Type>
struct LateLend {

	LateLend(): refer(nullptr) {
	}

	LateLend(std::nullptr_t null): refer(nullptr) {
	}

	LateLend(Own<Type> &early): refer(&early) {
	}

	LateLend(Lend<Type> lend): refer(&lend) {
	}


	LateLend(LateLend &cpy) = default;

	LateLend(LateLend &&mov) = default;


	void bind(Own<Type> &own) {
		refer = &own;
	}


	Lend<Type> operator() () const {

		if (refer == nullptr) {
			throw BadOwnership();
		}

		return *refer;
	}

private:

	Own<Type>* refer;
};

SNOW_OWL_NAMESPACE_END
