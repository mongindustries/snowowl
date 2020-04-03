//
// Created by micha on 3/31/2020.
//
#pragma once

#include <string>
#include <vector>

#include <headerconv.hpp>
#include <hidden_implem.hpp>
#include <window.hpp>
#include <ownership.hpp>

#include "context_backend.hpp"
#include "surface.hpp"
#include "screen.hpp"

SNOW_OWL_NAMESPACE(gx)

struct SWL_EXPORT Context {

	explicit Context(cx::Own<ContextBackend> backend);

	[[nodiscard]] std::string name() const {
		return backend->name();
	}

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

	cx::Own<ContextBackend> backend;
};

SNOW_OWL_NAMESPACE_END
