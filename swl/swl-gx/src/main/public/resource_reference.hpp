#pragma once

#include <header.hpp>

SNOW_OWL_NAMESPACE(gx)

/**
 * A resource reference construct
 *
 * A resource reference is an opaque structure with implementation specific to graphics APIs.
 *
 * To obtain a resource reference call any of the following methods:
 * - swap_chain's next_frame method
 * - buffer's reference method
 *
 * Resource references are managed by the object created them. Do not attempt to manage its
 * memory client side, it will lead to undefined behavior.
 */
struct resource_reference {

};

SNOW_OWL_NAMESPACE_END
