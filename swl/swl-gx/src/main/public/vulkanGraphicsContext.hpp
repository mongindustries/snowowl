//
// Created by Michael Ong on 6/4/20.
//
#pragma once

#include <headerconv.hpp>

#include "graphicsContext.hpp"
#include "windowSurface.hpp"

#include "vulkanImport.h"

SNOW_OWL_NAMESPACE(gx::implem)

struct VulkanGraphicsContext final: GraphicsContext {

	VulkanGraphicsContext();


	void makeSurface(ui::WindowSurface &surface) override;

private:

	vk::UniqueInstance _instance;

	vk::UniqueDevice _active_device;
};

SNOW_OWL_NAMESPACE_END
