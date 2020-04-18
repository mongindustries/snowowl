//
// Created by Michael Ong on 16/4/20.
//
#include <fstream>
#include <vector>

#include "vulkan_shader.hpp"
#include "vulkan_context.hpp"

using namespace std;
using namespace filesystem;

using namespace swl;
using namespace gx;

VulkanShader::VulkanShader(const VulkanGraphicsContext &context, const std::filesystem::path &location) {

	ifstream read{};

	read.open(location, ifstream::binary | ifstream::ate);

	auto size = read.tellg();

	vector<char> data(size);

	read.seekg(ios::beg);
	read.read (data.data(), size);

	shader = context._device->createShaderModuleUnique(vk::ShaderModuleCreateInfo({}, size, reinterpret_cast<uint32_t*>(data.data())));
}
