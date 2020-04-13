//
// Created by Michael Ong on 6/4/20.
//

#pragma once

#include <headerconv.hpp>
#include <ownership.hpp>

#include <point.hpp>
#include <core.hpp>

SNOW_OWL_NAMESPACE(ui)

struct SWL_EXPORT Window;

struct SWL_EXPORT WindowSurface {

	explicit WindowSurface(cx::Own<Window>& window);


	bool
		operator<   (const WindowSurface& rhs) const;

	bool
		operator==  (const WindowSurface &rhs) const;


	explicit operator void*() const {
		return _native_surface_handle;
	}

	[[nodiscard]] cx::Size2D getSize() const;

private:

	cx::Borrow<Window> _window;

	void*              _native_surface_handle{};
};

SNOW_OWL_NAMESPACE_END
