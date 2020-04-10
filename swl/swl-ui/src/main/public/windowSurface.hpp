//
// Created by Michael Ong on 6/4/20.
//

#pragma once

#include <headerconv.hpp>
#include <point.hpp>
#include <core.hpp>

SNOW_OWL_NAMESPACE(ui::backend)

struct WindowBackend;

SNOW_OWL_NAMESPACE_END

SNOW_OWL_NAMESPACE(ui)

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


	[[nodiscard]] cx::Size2D
		getSize         () const;


	[[nodiscard]] void*
		getNativeHandle () const { return _native_surface_handle; }

	[[nodiscard]] cx::DriverHandle
		getHandle       () const { return _handle; }


	bool
		operator<   (const WindowSurface& rhs) const;

	bool
		operator==  (const WindowSurface &rhs) const;


	friend struct backend::WindowBackend;

private:

	cx::DriverHandle  _handle{};


	const Window*     _window{};

	void*             _native_surface_handle{};
};

SNOW_OWL_NAMESPACE_END
