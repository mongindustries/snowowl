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

	~VulkanGraphicsContext();


	VulkanGraphicsContext(VulkanGraphicsContext&& mov) noexcept;

	VulkanGraphicsContext& operator=(VulkanGraphicsContext&& mov) noexcept;


	void makeSurface(ui::WindowSurface &surface) const override;



	vk::Instance const&  getInstance
		() const { return _instance.get(); }

	vk::Device const&    getActiveDevice
		() const { return _active_device.get(); }
	
	
private:

	vk::DynamicLoader*  loader;
	
	vk::UniqueInstance  _instance;

	vk::UniqueDevice    _active_device;
};

SNOW_OWL_NAMESPACE_END
