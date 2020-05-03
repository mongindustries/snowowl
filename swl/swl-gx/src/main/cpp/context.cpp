//
// Created by Michael Ong on 25/4/20.
//
#include "context.hpp"

#include "queue.hpp"
#include "swap_chain.hpp"

SNOW_OWL_NAMESPACE(gx)

context::context      () noexcept = default;

context::context      (context &&move) noexcept = default;

context::~context     () = default;

context&
  context::operator=  (context &&) noexcept { return *this; }


SNOW_OWL_NAMESPACE_END
