//
// Created by Michael Ong on 1/4/20.
//
#pragma once

#include "headerconv.hpp"

#include <utility>
#include <type_traits>
#include <functional>

SNOW_OWL_NAMESPACE(cx)

struct BadOwnership: std::exception { };

/// Definite upon-instantiation borrowed ownership.
template<typename Type>
using Lend = Type const&;

/**
 * An object that holds ownership over a pointer.
 *
 * This is similar to <code>std's</code> <code>unique_ptr</code> construct wherein
 * <code>Own</code>'s ownership can only be moved.
 *
 * But it is different from <code>unique_ptr</code> as it can be borrowd through
 * <code>Lend</code> at any time.
 *
 * @tparam Type The type that <code>Own</code> holds.
 */
template<typename Type, typename std::enable_if_t< std::is_class_v<Type>, int> = 0>
struct Own {

	Own(const Own &cpy) = delete; // own cannot be copied, selfish siya eh

	Own(Own &&mov) noexcept : value(mov.value) {
		mov.value = nullptr;
	}


	explicit Own(Type* fast): value(fast) { } // fast init, does not destruct Type.

	template< typename Param1, typename... ParamRemain >
	Own(Param1 param1, ParamRemain... parameters): value(new Type(param1, parameters...)) { }

	template<typename Derive, std::enable_if_t< std::is_base_of_v< Type, Derive >, int > = 0>
	Own(Derive &&move): value(new Derive(std::forward<Derive>(move))) { } // slow but terse init, destructs Type upon move.


	~Own() {
		if (value != nullptr) {
			delete value;
		}
	}


	[[nodiscard]] bool isValid() const {
		return value != nullptr;
	}

	void mutate(std::function<void(Type* const)> mutate) {
		mutate(value);
	}

	// access

	Type *const operator->() const {

		if (!isValid()) {
			throw BadOwnership();
		}

		return value;
	}

	Lend<Type>  operator* () const {

		if (!isValid()) {
			throw BadOwnership();
		}

		return *value;
	}

	// assignments

	Own&  operator= (const Own &copy) = delete;

	Own&  operator= (Own &&move) noexcept {
		value = move.value;
		move.value = nullptr;

		return *this;
	}

	// comparison

	bool operator==(const Type* rhs) const {
		return *value == *rhs;
	}

private:

	Type *value;
};

/// Definite ownership mutating consumer.
template<typename Type>
using MutateOwn = Own<Type>&;

template<typename Impl, typename... Params>
Own<Impl> make_own(Params... params) {
	return Own(new Impl(params...));
}

/// Late binding borrowed ownership. Use <code>bind()</code> to indicate lending.
template<typename Type>
struct LateLend {

	LateLend(): refer(nullptr) {
	}

	explicit LateLend(std::nullptr_t null): refer(nullptr) {
	}

	explicit LateLend(Own<Type> &early): refer(&early) {
	}

	explicit LateLend(Lend<Type> lend): refer(&lend) {
	}


	LateLend(LateLend &cpy) = default;

	LateLend(LateLend &&mov) noexcept = default;


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
