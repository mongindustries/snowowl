//
// Created by Michael Ong on 31/3/20.
//
#pragma once

#include <headerconv.hpp>
#include <hidden_implem.hpp>

SNOW_OWL_NAMESPACE(gx)

struct DeviceBackend {

	struct Context;


	DeviceBackend(const DeviceBackend &cpy) = delete;

	DeviceBackend(DeviceBackend &&mov) = default;


private:

	cx::Himplem<Context> context;
};

SNOW_OWL_NAMESPACE_END
