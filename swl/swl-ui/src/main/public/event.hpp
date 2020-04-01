//
// Created by Michael Ong on 31/3/20.
//
#pragma once

#include <functional>
#include <utility>

#include <headerconv.hpp>

SNOW_OWL_NAMESPACE(ui)

// master event bus, for general case: using this will lead to a crash.

template<typename Return, typename... Parameters>
struct Event {

	template<typename Scope, typename = std::enable_if<std::is_object_v<std::decay<Scope>>>>
	using member_function = Return (Scope::*)(Parameters...);

	Event() = default;

	explicit Event(std::function<Return(Parameters...)> functor) {

	}

	template<typename Scope>
	Event(Scope *target, member_function<Scope> functor) {

	}
};

//

template<typename Return, typename Param>
struct Event<Return, Param> {

	template<typename Scope, typename = std::enable_if<std::is_object_v<std::decay<Scope>>>>
	using member_function = Return (Scope::*)(Param);

	Event() = default;

	explicit Event(std::function<Return(Param)> functor) {

	}

	template<typename Scope>
	Event(Scope*, member_function<Scope> memberFunction) {

	}

	Return invoke(Param param1) {
	}
};

template<typename Return, typename Param1, typename Param2>
struct Event<Return, Param1, Param2> {

	template<typename Scope, typename = std::enable_if<std::is_object_v<std::decay<Scope>>>>
	using member_function = Return (Scope::*)(Param1, Param2);

	Event() = default;

	explicit Event(std::function<Return(Param1, Param2)> functor) {

	}

	template<typename Scope>
	Event(Scope*, member_function<Scope> memberFunction) {

	}

	Return invoke(Param1 param1, Param2 param2) {
	}
};

template<typename Return, typename Param1, typename Param2, typename Param3>
struct Event<Return, Param1, Param2, Param3> {

	template<typename Scope, typename = std::enable_if<std::is_object_v<std::decay<Scope>>>>
	using member_function = Return (Scope::*)(Param1, Param2, Param3);

	Event() = default;

	explicit Event(std::function<Return(Param1, Param2)> functor) {

	}

	template<typename Scope>
	Event(Scope*, member_function<Scope> memberFunction) {

	}

	Return invoke(Param1 param1, Param2 param2, Param3 param3) {
	}
};

SNOW_OWL_NAMESPACE_END
