//
// Created by Michael Ong on 6/4/20.
//

#pragma once

#include <header.hpp>
#include <ownership_exp.hpp>

#include <point.hpp>
#include <core.hpp>

SNOW_OWL_NAMESPACE(ui)

struct SWL_EXPORT Window;

struct SWL_EXPORT WindowSurface {

	explicit WindowSurface(cx::exp::ptr<Window>& window);


	bool
		operator<   (const WindowSurface& rhs) const;

	bool
		operator==  (const WindowSurface &rhs) const;


	explicit operator void*() const {
		return _native_surface_handle;
	}

	[[nodiscard]] cx::size_2d getSize() const;

	[[nodiscard]] cx::exp::ptr_ref<Window> getWindow() const;

private:

	cx::exp::ptr_ref<Window> _window;

	void* _native_surface_handle{};
};

SNOW_OWL_NAMESPACE_END
