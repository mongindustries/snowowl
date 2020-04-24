//
// Created by Michael Ong on 22/4/20.
//
#include "layer.hpp"

using namespace std;
using namespace swl;

rd::layer::layer(const std::string& name, rd::layer_type layerType) {

}

rd::layer::layer(layer &&mov) noexcept: camera(mov.camera), entities(std::move(mov.entities)) {
	mov.entities.clear();
}

void  rd::layer::bind_renderer(const cx::driver_handle &reference) {
	bound_renderer = reference;
}
