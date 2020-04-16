//
// Created by Michael Ong on 16/4/20.
//
#pragma once

#include <filesystem>

#include <header.hpp>
#include "vulkanImport.h"

SNOW_OWL_NAMESPACE(gx)

struct VulkanGraphicsContext;

struct VulkanShader {

	vk::UniqueShaderModule shader;

	VulkanShader(const VulkanGraphicsContext &context, const std::filesystem::path &location);
};

SNOW_OWL_NAMESPACE_END
