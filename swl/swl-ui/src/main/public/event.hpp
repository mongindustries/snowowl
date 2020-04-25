//
// Created by Michael Ong on 31/3/20.
//
#pragma once

#include <functional>
#include <header.hpp>

SNOW_OWL_NAMESPACE(ui)

template<typename Return, typename... Parameters>
struct event {

	event             () = default;

	explicit event    (std::function<Return(Parameters...)> functor):
				_fn(functor) { }


	Return
				invoke      (Parameters... params) const {
		return _fn(params...);
	}

	Return
				operator()  (Parameters... params) const {
		return invoke(params...);
	}

private:

	std::function<Return(Parameters...)> _fn;
};

template<typename... Parameters>
struct event<void, Parameters...> {

	event             () = default;

	explicit event    (std::function<void(Parameters...)> functor) : _fn(functor) {

	}


	void  invoke      (Parameters... params) const {
		_fn(params...);
	}

	void  operator()  (Parameters... params) const {
		invoke(params...);
	}

private:

	std::function<void(Parameters...)> _fn;
};

SNOW_OWL_NAMESPACE_END
