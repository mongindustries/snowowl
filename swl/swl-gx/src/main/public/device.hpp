//
// Created by micha on 3/31/2020.
//
#pragma once

#include <string>
#include <vector>

#include <headerconv.hpp>
#include <hidden_implem.hpp>
#include <window.hpp>

#include "device_backend.hpp"
#include "surface.hpp"
#include "screen.hpp"

SNOW_OWL_NAMESPACE(gx)

struct Device {

	explicit Device(DeviceBackend backend);

	[[nodiscard]] std::string name() const;

	// device properties

	Screen mainScreen() const;

	std::vector<Screen> screens() const;

	// ops for creating device-bound / cpu-bound resource

	/**
	 * Returns the window's drawing surface.
	 * @param window The window to obtain the surface.
	 * @return A <code>Surface</code> object that holds the reference to the surface.
	 */
	Surface createSurface(ui::Window &window);

	// ops for committing queues

	// ops for synchornizing stuff

private:

	DeviceBackend backend;
};

SNOW_OWL_NAMESPACE_END
