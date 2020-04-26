//
// Created by Michael Ong on 26/4/20.
//
#import "metal/mtl_buffer.h"

SNOW_OWL_NAMESPACE(gx::mtl)

mtl_buffer::mtl_buffer(const cx::exp::ptr_ref<mtl_context> &context, location locality, size_t size) :
	graphics_buffer<graphics_buffer_type::typeData>(context.cast<graphics_context>(), locality, size) {

	MTLResourceOptions resource_options;

	switch(locality) {
		case location::both: // managed macos / shared ios
			break;
		case location::host: // shared
			break;
		case location::local: // private
			break;
	}
}

mtl_buffer::mtl_buffer(const cx::exp::ptr_ref<mtl_context> &context, location locality, const std::vector<char> &buffer) :
	graphics_buffer<graphics_buffer_type::typeData>(context.cast<graphics_context>(), locality, buffer) {

	switch(locality) {
		case location::both:
			break;
		case location::host:
			break;
		case location::local:
			break;
	}
}

void mtl_buffer::update_data  (unsigned long start, const std::vector<char> &data) {

}

void mtl_buffer::stage        (const cx::exp::ptr_ref<graphics_queue> &staging_queue) {

}

SNOW_OWL_NAMESPACE_END
