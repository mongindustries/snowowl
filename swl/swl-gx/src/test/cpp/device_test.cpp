//
// Created by Michael Ong on 1/4/20.
//
#include "device.hpp"
#include "device_backend_vulkan.hpp"

#include <gtest/gtest.h>

namespace SWL {
	using namespace swl::cx;
	using namespace swl::gx;
	using namespace swl::gx::backend;
};

using namespace SWL;

TEST(Device, ShouldConstruct) {

	Device device { DeviceBackendVulkan() };

	auto deviceName = device.name();

	ASSERT_EQ(deviceName, "vulkan");
}
