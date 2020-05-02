//
// Created by Michael Ong on 22/4/20.
//
//
#pragma once

#include <type_traits>
#include <memory>

#include "header.hpp"

SNOW_OWL_NAMESPACE(cx::exp)

/**
 * A self-deleting pointer wrapper. This version contains type information
 * of the object's base class.
 * @tparam Base The base object of Derive.
 * @tparam Derive The object this pointer will manage.
 */
template<typename Base, typename Derive = void>
struct ptr {

  typedef ptr<Base, Derive>& reference;

	static_assert(std::is_base_of_v<Base, Derive>, "Derive must be inherited from Base!");


	ptr                 (const ptr<Base, Derive>& cpy) = delete;

	reference operator= (const ptr<Base, Derive>& cpy) = delete;


	ptr(ptr<Base, Derive>&& mov) noexcept : _value(mov._value) {
		mov._value = nullptr;
	}

	ptr<Base, Derive>& operator=(ptr<Base, Derive>&& mov) noexcept {

		_value = mov._value;
		mov._value = nullptr;

		return *this;
	}


	/**
	 * Creates a new <code>ptr</code> instance.
	 */
	explicit ptr(): _value(new Derive()) { }

	/**
	 * Creates a new <code>ptr</code> instance with a null-backed value.
	 * @param null
	 */
	explicit ptr(std::nullptr_t null): _value(null) { }

	/**
	 * Creates a new <code>ptr</code> instance from a previously declared pointer
	 * @param fast The pointer created. Take note <code>fast</code> should not been made using another <code>ptr</code>
	 * instance. If it does, call <code>release</code> or <code>abstract_and_release</code> before passing
	 * the pointer here.
	 */
	explicit ptr(Derive* fast): _value(fast) { }

	/**
	 * Creates a new <code>ptr</code> instance forward-declared from a <code>Derive</code> value.
	 * @param fwd The object to forward to <code>ptr</code>.
	 * @remarks Use <code>std::move</code> if you are not going to use the object (or just directly declare it inside
	 * the argument).
	 */
	explicit ptr(Derive &&fwd): _value(new Derive(std::forward<Derive>(fwd))) { }


	~ptr() {
		reset();
	}

	operator ptr<Base>() {
		return abstract();
	}

	Base* abstract_pointer() const {
		return static_cast<Base*>(_value);
	}

	Base* abstract_and_release() {
		Base* ptr = abstract_pointer();
		_value = nullptr;
		return ptr;
	}

	ptr<Base, void> abstract() {
		return ptr<Base, void>{ abstract_and_release() };
	}


	[[nodiscard]] bool is_valid() const {
		return _value != nullptr;
	}

	explicit operator bool() const { return is_valid(); }


	[[nodiscard]] Derive& get() const { return *_value; }

	operator Derive&() const { return get(); }


	Derive* operator->() const { return pointer(); }

	Derive* pointer() const { return _value; }


	bool operator==(const ptr<Base, Derive>& rhs) const {
		return *_value == *rhs._value;
	}

	bool operator==(const Derive& rhs) const {
		return *_value == rhs;
	}


	Derive* release() {
		Derive* ptr = _value;
		_value = nullptr;
		return ptr;
	}

	void reset() {
		if (is_valid()) {
			delete _value;
		}

		_value = nullptr;
	}

private:

	Derive* _value;
};

template<typename Base>
struct ptr<Base, void> {

	ptr(const ptr<Base>& cpy) = delete;

	ptr<Base>& operator=(const ptr<Base>& cpy) = delete;


	ptr(ptr<Base>&& mov) noexcept : _value(mov._value) {
		mov._value = nullptr;
	}

	ptr<Base>& operator=(ptr<Base>&& mov) noexcept {

		_value = mov._value;
		mov._value = nullptr;

		return *this;
	}


	explicit ptr(): _value(new Base()) { }

	explicit ptr(std::nullptr_t null): _value(null) { }

	explicit ptr(Base* fast): _value(fast) { }

	explicit ptr(Base &&fwd): _value(new Base(std::forward<Base>(fwd))) { }


	~ptr() {
		reset();
	}


	template<typename Derive>
	std::enable_if_t<std::is_base_of_v<Base, Derive>, Derive>* reify_pointer() {
		Derive* ptr = static_cast<Derive*>(_value);
		return ptr;
	}

	template<typename Derive>
	std::enable_if_t<std::is_base_of_v<Base, Derive>, Derive>* reify_and_release() {
		Derive* ptr = reify_pointer<Derive>(_value);
		_value = nullptr;
		return ptr;
	}

	template<typename Derive>
	std::enable_if_t<std::is_base_of_v<Base, Derive>, ptr<Base, Derive>> reify() const {
		return ptr<Base, Derive>{ reify_and_release<Derive>(_value) };
	}

	template<typename Derive>
	std::enable_if_t<std::is_base_of_v<Base, Derive>, ptr<Derive>> cast() const {
		return ptr<Derive> { reify_and_release<Derive>(_value)._value };
	}


	[[nodiscard]] bool is_valid() const {
		return _value != nullptr;
	}

	explicit operator bool() const { return is_valid(); }


	[[nodiscard]] Base& get() const { return *_value; }

	operator Base&() const { return get(); }


	Base* operator->() const { return pointer(); }

	Base* pointer() const { return _value; }


	bool operator==(const ptr<Base>& rhs) const {
		return *_value == *rhs._value;
	}

	template<typename Anything>
	bool operator==(const Anything& rhs) const {
		return *_value == rhs;
	}

	Base* release() {
		Base* ptr = _value;
		_value = nullptr;
		return ptr;
	}

	void reset() {
		if (is_valid()) {
			delete _value;
		}

		_value = nullptr;
	}

private:

	Base* _value;
};


template<typename ClassType>
struct ptr_ref {

	ptr_ref         (): _value(nullptr) { }

	explicit ptr_ref(std::nullptr_t null): _value(null) { }

	template<typename Base>
	explicit ptr_ref(const ptr<Base, ClassType> &ref): _value(ref.pointer()) { }

	explicit ptr_ref(const ptr<ClassType> &ref): _value(ref.pointer()) { }

	explicit ptr_ref(ClassType* ptr): _value(ptr) { }


	[[nodiscard]] bool is_valid() const {
		return _value != nullptr;
	}

	explicit operator bool() const { return is_valid(); }


	[[nodiscard]] ClassType& get() const { return *_value; }

	operator ClassType&() const { return get(); }


	ClassType* operator->() const { return pointer(); }

	ClassType* pointer() const { return _value; }


	bool operator==(const ptr_ref<ClassType>& rhs) const {
		return *_value == *rhs._value;
	}

	template<typename Anything>
	bool operator==(const Anything& rhs) const {
		return *_value == rhs;
	}


	template<typename Anything>
	ptr_ref<Anything> cast() const {
		return ptr_ref<Anything> { static_cast<Anything*>(_value) };
	}

private:

	ClassType* _value;
};

template<>
struct ptr_ref<void> {

	explicit ptr_ref(std::nullptr_t null): _value(null) { }

	explicit ptr_ref(void* ptr): _value(ptr) { }


	[[nodiscard]] bool is_valid() const {
		return _value != nullptr;
	}

	explicit operator bool() const { return is_valid(); }


	template<typename Anything>
	ptr_ref<Anything> cast() const {
		return ptr_ref<Anything> { static_cast<Anything*>(_value) };
	}

	void* pointer() const {
		return _value;
	}

private:

	void* _value;
};

SNOW_OWL_NAMESPACE_END
