//
// Created by Michael Ong on 6/4/20.
//
#pragma once

#include <map>
#include <vector>

#include <headerconv.hpp>

#include "vulkanImport.h"
#include "windowSurface.hpp"

SNOW_OWL_NAMESPACE(gx::backend)

struct VulkanGraphicsBackend {

	static VulkanGraphicsBackend* instance;

	static std::vector<const char*> vulkanExtensions;


	void makeSurface(vk::Instance &instance, ui::WindowSurface &surface);

private:

	std::map<std::reference_wrapper<ui::WindowSurface>, vk::SurfaceKHR, std::less<const ui::WindowSurface>> surfaces;
};

SNOW_OWL_NAMESPACE_END
