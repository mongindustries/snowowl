//
// Created by Michael Ong on 6/4/20.
//
#pragma once

#include <map>
#include <vector>

#include <headerconv.hpp>

#include "vulkanImport.h"
#include "vulkanGraphicsContext.hpp"

#include "windowSurface.hpp"

SNOW_OWL_NAMESPACE(gx::backend)

struct VulkanGraphicsBackend {

	static VulkanGraphicsBackend* instance;

	
	static std::vector<const char*> vulkanExtensions;

	static std::vector<const char*> vulkanLayers;


	void makeSurface(vk::Instance const &instance, ui::WindowSurface &surface);

	friend struct gx::implem::VulkanGraphicsContext;

private:

	std::map<std::reference_wrapper<ui::WindowSurface>, vk::SurfaceKHR, std::less<const ui::WindowSurface>> surfaces;
};

SNOW_OWL_NAMESPACE_END
