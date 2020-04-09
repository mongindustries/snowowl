//
// Created by Michael Ong on 6/4/20.
//
#pragma once

#include <headerconv.hpp>

#include "vulkanImport.h"
#include "windowSurface.hpp"

#include "graphicsContext.hpp"

SNOW_OWL_NAMESPACE(gx::implem)

struct VulkanGraphicsContext final: GraphicsContext {

	VulkanGraphicsContext();

	~VulkanGraphicsContext();


	VulkanGraphicsContext(VulkanGraphicsContext&& mov) noexcept;

	VulkanGraphicsContext& operator=(VulkanGraphicsContext&& mov) noexcept;


	void makeSurface(ui::WindowSurface &surface) const override;

	vk::SurfaceKHR const& getSurface(ui::WindowSurface const &surface) const;



	vk::Instance const&          getInstance
		() const { return _instance.get(); }

	vk::PhysicalDevice const&    getActiveDevice
		() const { return _active_device; }


	
	
private:

	vk::DynamicLoader*  loader;
	
	vk::UniqueInstance  _instance;

	vk::PhysicalDevice  _active_device;
};

SNOW_OWL_NAMESPACE_END
