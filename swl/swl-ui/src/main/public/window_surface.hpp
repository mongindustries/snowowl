//
// Created by Michael Ong on 6/4/20.
//

#pragma once

#include <header.hpp>
#include <ownership_exp.hpp>

#include <point.hpp>
#include <core.hpp>

#include "window.hpp"

SNOW_OWL_NAMESPACE(ui)

struct SWL_EXPORT window;

struct SWL_EXPORT window_surface {

	explicit window_surface(const cx::exp::ptr_ref<window>& window);

	bool  operator<   (const window_surface& rhs) const;

	bool  operator==  (const window_surface &rhs) const;


	[[nodiscard]] cx::size_2d
				get_size    () const;

	[[nodiscard]] cx::exp::ptr_ref<window>
				get_window  () const;


	template<typename Anything>
	cx::exp::ptr_ref<Anything>
				cast        () const {
		return surface_handle.cast<Anything>();
	}

private:

	cx::exp::ptr_ref<window>  window;

	cx::exp::ptr_ref<void>    surface_handle{nullptr };
};

SNOW_OWL_NAMESPACE_END
