//
// Created by Michael Ong on 31/3/20.
//
#pragma once

#include <string>
#include <headerconv.hpp>
#include <hidden_implem.hpp>

SNOW_OWL_NAMESPACE(gx)

struct DeviceBackend {

	DeviceBackend() = default;

	virtual ~DeviceBackend() = default;


	virtual std::string name() const = 0;
};

SNOW_OWL_NAMESPACE_END
