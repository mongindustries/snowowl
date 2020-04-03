//
// Created by Michael Ong on 1/4/20.
//
#include "context.hpp"
#include "context_backend_vulkan.hpp"

#include <iostream>
#include <gtest/gtest.h>

namespace SWL {
	using namespace swl::cx;
	using namespace swl::gx;
	using namespace swl::gx::backend;
};

using namespace std;
using namespace SWL;

TEST(VulkanContext, ShouldConstruct) {

	Context device { ContextBackendVulkan() };

	auto deviceName = device.name();

	cout << "Backend device: " << deviceName << endl;

	ASSERT_EQ(deviceName, "vulkan");
}
