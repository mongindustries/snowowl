//
// Created by Michael Ong on 31/3/20.
//
#pragma once

#include <headerconv.hpp>

#include <data.hpp>
#include <window.hpp>

SNOW_OWL_NAMESPACE(gx)

struct Surface {

	Surface(ui::Window &window);


	cx::Data contents() const noexcept;

	cx::MutableData mutableContents() const;


	void surfaceUpdated(const ui::Window &window, cx::Rect rect);

	void surfaceDestroy(const ui::Window &window);
};

SNOW_OWL_NAMESPACE_END
