//
// Created by Michael Ong on 31/3/20.
//
#pragma once

#include <headerconv.hpp>

#include <data.hpp>
#include <window.hpp>

#include "graphics.hpp"

SNOW_OWL_NAMESPACE(gx)

struct Surface {

	Surface(ui::Window &window);


	[[nodiscard]] cx::Data contents() const noexcept;

	[[nodiscard]] cx::MutableData mutableContents() const;


	void surfaceUpdated(const ui::Window &window, cx::Rect rect);

	void surfaceDestroy(const ui::Window &window);

private:

	DriverHandle handle;
};

SNOW_OWL_NAMESPACE_END
