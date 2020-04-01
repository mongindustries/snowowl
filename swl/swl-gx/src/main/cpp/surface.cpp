//
// Created by Michael Ong on 31/3/20.
//

#include "surface.hpp"

using namespace std;

using namespace swl::cx;
using namespace swl::gx;
using namespace swl::ui;

Surface::Surface(swl::ui::Window &window) {

	window.addEventFrame({ this, &Surface::surfaceUpdated });
	window.addEventClose({ this, &Surface::surfaceDestroy });
}


void Surface::surfaceDestroy(const Window &window) {
}

void Surface::surfaceUpdated(const Window &window, Rect rect) {
}

Data Surface::contents() const noexcept {
	return Data(nullptr, 0);
}

MutableData Surface::mutableContents() const {
	return MutableData(nullptr, 0);
}
