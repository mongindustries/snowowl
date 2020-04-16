//
// Created by Michael Ong on 6/4/20.
//
#pragma once

#include <map>
#include <vector>

#include <header.hpp>

#include "vulkanImport.h"
#include "vulkanGraphicsContext.hpp"

#include "windowSurface.hpp"

SNOW_OWL_NAMESPACE(gx::backend)

struct VulkanGraphicsBackend {

	static VulkanGraphicsBackend* instance;

	
	static std::vector<const char*> vulkanExtensions;

	static std::vector<const char*> vulkanLayers;


	vk::UniqueSurfaceKHR makeSurface
		(vk::Instance const &vk_instance, const ui::WindowSurface &surface);


	friend struct gx::VulkanGraphicsContext;
};

SNOW_OWL_NAMESPACE_END
