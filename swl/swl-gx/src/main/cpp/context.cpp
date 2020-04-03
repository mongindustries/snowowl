//
// Created by micha on 3/31/2020.
//

#include "context.hpp"

#include "swl_vulkan.hpp"

using namespace swl::cx;
using namespace swl::gx;
using namespace swl::ui;

using namespace std;

Context::Context(cx::Own<ContextBackend> backend): backend(move(backend)) {

}

Surface Context::createSurface(Window &window) {
	(void) backend;
	return Surface(window);
}


Screen Context::mainScreen() const {
	return { };
}

std::vector<Screen> Context::screens() const {
	(void) backend;
	return { };
}
