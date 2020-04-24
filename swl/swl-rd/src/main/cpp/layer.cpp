//
// Created by Michael Ong on 22/4/20.
//
#include "layer.hpp"

using namespace std;
using namespace swl;

SNOW_OWL_NAMESPACE(rd)

layer::layer
			(const std::string& name, layer_type layer_type) { }

layer::layer
			(layer&& mov) noexcept : camera(mov.camera), bound_renderer(mov.bound_renderer), entities(std::move(mov.entities)) { }


layer::~layer
			() = default;

layer&
			layer::operator=(layer&& mov) noexcept {

	camera          = mov.camera;
	bound_renderer  = mov.bound_renderer;
	entities        = std::move(mov.entities);
	
	return *this;
}

void  layer::bind_renderer(const cx::driver_handle &reference) {
	bound_renderer = reference;
}

SNOW_OWL_NAMESPACE_END
