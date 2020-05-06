//
// Created by Michael Ong on 4/3/2020.
//
#include "application.hpp"

#include "swl_window_backend.hpp"
#include "window.hpp"

using namespace std;
using namespace swl;

SNOW_OWL_NAMESPACE(ui)

using namespace backend;

application::application  () noexcept { }

application::application  (void* native_instance) : native_instance(native_instance) {
  window_backend::instance->application = this;
}

application::~application () = default;

SNOW_OWL_NAMESPACE_END
