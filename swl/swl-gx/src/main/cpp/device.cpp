//
// Created by micha on 3/31/2020.
//

#include "device.hpp"

#include "swl_vulkan.hpp"
#include "swl_vulkan_mac.hpp"

using namespace swl::gx;
using namespace swl::gx_private;
using namespace swl::ui;

using namespace std;

Device::Device(DeviceBackend backend): backend(move(backend)) {

}

string Device::name() const {
	return ::string();
}

Surface Device::createSurface(Window &window) {
	return Surface(window);
}
