//
// Created by Michael Ong on 6/4/20.
//

#pragma once

#include <functional>

#include <headerconv.hpp>
#include <core.hpp>

SNOW_OWL_NAMESPACE(ui)

namespace backend {
		struct SWL_EXPORT WindowBackend;
}

struct SWL_EXPORT Window;

struct SWL_EXPORT WindowSurface {

	WindowSurface
		(const WindowSurface &copy) = delete;

	WindowSurface& operator=
		(const WindowSurface &copy) const = delete;


	WindowSurface
		(WindowSurface &&mov) noexcept;

	WindowSurface& operator=
		(WindowSurface &&mov) noexcept;


	WindowSurface     () = default;

	~WindowSurface    ();


	void
		presentAndWait  ();


	void*
		getNativeHandle () const { return _native_surface_handle; }

	cx::DriverHandle
		getHandle       () const { return _handle; }


	bool
		operator<   (const WindowSurface& rhs) const;

	bool
		operator==  (const WindowSurface &rhs) const;


	friend struct backend::WindowBackend;

private:

	cx::DriverHandle  _handle;


	const Window*     _window;

	void*             _native_surface_handle;
};

SNOW_OWL_NAMESPACE_END
