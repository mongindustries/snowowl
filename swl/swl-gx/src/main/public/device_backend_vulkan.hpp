//
// Created by Michael Ong on 1/4/20.
//
#pragma once

#include <headerconv.hpp>
#include "device_backend.hpp"

SNOW_OWL_NAMESPACE(gx :: backend)

struct DeviceBackendVulkan: public DeviceBackend {

	struct Context;

	cx::Himplem<Context> context;


	DeviceBackendVulkan();

	DeviceBackendVulkan(DeviceBackendVulkan &&mov);


	std::string name() const override;
};

SNOW_OWL_NAMESPACE_END
