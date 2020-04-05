//
// Created by Michael Ong on 31/3/20.
//
#pragma once

#include <functional>
#include <utility>

#include <headerconv.hpp>

SNOW_OWL_NAMESPACE(ui)

template<typename Return, typename... Parameters>
struct Event {

	Event() = default;

	explicit Event(std::function<Return(Parameters...)> functor): _fn(functor) {

	}

	template<typename Scope>
	Event(Scope *target, Return (Scope::*functor)(Parameters...)) {
		_fn = [target, &functor](Parameters... params) -> Return {
			if (target != nullptr) {
				return (target->*functor)(params...);
			}
		};
	}

	inline Return invoke(Parameters... params) const {
		return _fn(params...);
	}

	Return operator() (Parameters... params) const {
		return invoke(params...);
	}

private:

	std::function<Return(Parameters...)> _fn;
};

SNOW_OWL_NAMESPACE_END
