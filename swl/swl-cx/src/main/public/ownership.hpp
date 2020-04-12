//
// Created by Michael Ong on 1/4/20.
//
#pragma once

#include "headerconv.hpp"

#include <utility>
#include <type_traits>
#include <functional>

SNOW_OWL_NAMESPACE(cx)

template<typename ClassType>
struct Own {

	explicit Own(const Own<ClassType>& copy) = delete;

	Own<ClassType>& operator=(const Own<ClassType>& copy) = delete;


	Own(Own<ClassType>&& mov) noexcept;

	Own<ClassType>& operator=(Own<ClassType>&& mov) noexcept;


	template<typename Derive, std::enable_if_t< std::is_base_of<ClassType, Derive>::value, int> = 0>
	explicit Own(Derive* fast); // fast init

	template<typename Derive, std::enable_if_t< std::is_base_of<ClassType, Derive>::value, int> = 0>
	Own<ClassType>& operator=(Derive* fast);
	

	explicit Own(std::nullptr_t) noexcept : _value(nullptr) { }


	template<typename Param1, typename... Parameters>
	explicit Own(Param1 param1, Parameters... params);

	template<typename Derive, std::enable_if_t< std::is_base_of<ClassType, std::decay_t< Derive >>::value, int> = 0>
	explicit Own(const Derive& forward) noexcept;


	~Own();


	ClassType* const operator-> ();

	template<typename Derive = ClassType>
	Derive& get                 () const;


	explicit operator bool      () const {
		return is_valid();
	}

	
	template<typename Derive, std::enable_if_t< std::is_base_of_v<ClassType, Derive>, int> = 0>
	bool operator==             (const Own<Derive>& rhs) const;

	template<typename Derive, std::enable_if_t< std::is_base_of_v<ClassType, Derive>, int> = 0>
	bool operator==             (const Derive& rhs) const;

	template<typename Derive, std::enable_if_t< std::is_base_of_v<ClassType, Derive>, int> = 0>
	bool operator==             (const Derive* rhs) const;


	[[nodiscard]] bool is_valid () const;

private:
	ClassType* _value{};
};

template<typename ClassType>
struct Borrow {

	explicit Borrow(Own<ClassType>&);


	ClassType const& operator()() const;

private:

	Own<ClassType> const& _borrow;
};

template<typename ClassType>
struct MutableBorrow {

	explicit MutableBorrow(Own<ClassType>&);

	ClassType& operator()() const;

private:

	ClassType* _mutable_borrow;
};

// MARK: Own IMPLEMENTATION

template <typename ClassType>
Own<ClassType>::Own(Own< ClassType >&& mov) noexcept : _value(std::move(mov._value)) { mov._value = nullptr; }

template <typename ClassType>
template <typename Param1, typename ... Parameters>
Own<ClassType>::Own(Param1 param1, Parameters... params) : _value(new ClassType(param1, params...)) { }

template <typename ClassType>
template <typename Derive, std::enable_if_t<std::is_base_of<ClassType, Derive>::value, int>>
Own<ClassType>::Own(Derive* fast) : _value(fast) { }

template <typename ClassType>
template <typename Derive, std::enable_if_t<std::is_base_of<ClassType, Derive>::value, int>>
Own<ClassType>& Own<ClassType>::operator=(Derive* fast) {
	if (_value) {
		delete _value;
	}

	_value = fast;

	return *this;
}

template <typename ClassType>
template <typename Derive, std::enable_if_t<std::is_base_of<ClassType, std::decay_t<Derive>>::value, int>>
Own<ClassType>::Own(const Derive& forward) noexcept : _value(new Derive(std::move(forward))) { }


template <typename ClassType>
Own<ClassType>::~Own() {
	if (_value) {
		delete _value;
	}
}

template <typename ClassType>
ClassType* const Own<ClassType>::operator->() {
	return this->_value;
}

template <typename ClassType>
template <typename Derive>
Derive& Own<ClassType>::get() const {
	return *static_cast<Derive*>(this->_value);
}

template <typename ClassType>
template <typename Derive, std::enable_if_t<std::is_base_of_v<ClassType, Derive>, int>>
bool Own<ClassType>::operator==(const Own<Derive>& rhs) const {
	return *_value == *rhs();
}

template <typename ClassType>
template <typename Derive, std::enable_if_t<std::is_base_of_v<ClassType, Derive>, int>>
bool Own<ClassType>::operator==(const Derive& rhs) const {
	return rhs == *_value;
}

template <typename ClassType>
template <typename Derive, std::enable_if_t<std::is_base_of_v<ClassType, Derive>, int>>
bool Own<ClassType>::operator==(const Derive* rhs) const {
	return *rhs == *_value;
}

template <typename ClassType>
bool Own<ClassType>::is_valid() const {
	return _value != nullptr;
}

template <typename ClassType>
Own<ClassType>& Own<ClassType>::operator=(Own<ClassType>&& mov) noexcept {

	_value = mov._value;
	mov._value = nullptr;

	return *this;
}

// MARK: Borrow Implementation

template <typename ClassType>
Borrow<ClassType>::Borrow(Own<ClassType> &to_borrow): _borrow(to_borrow) {}

template <typename ClassType>
ClassType const& Borrow<ClassType>::operator()() const {
	return const_cast<ClassType const&>(*_borrow());
}

// MARK: Mutable Borrow Implementation

template <typename ClassType>
MutableBorrow<ClassType>::MutableBorrow(Own<ClassType> &to_borrow): _mutable_borrow(&to_borrow.get()) {}

template <typename ClassType>
ClassType& MutableBorrow<ClassType>::operator()() const {
	return *_mutable_borrow;
}

SNOW_OWL_NAMESPACE_END
