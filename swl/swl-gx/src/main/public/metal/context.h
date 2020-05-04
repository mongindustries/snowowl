//
//  context.h
//  SnowOwl
//
//  Created by Michael Ong on 4/5/20.
//  Copyright © 2020 mong Industries. All rights reserved.
//
#pragma once

#include <header.hpp>

#include "context.hpp"

SNOW_OWL_NAMESPACE(gx::mtl)

struct context: gx::context {

  context();

  OBJC_ID(MTLDevice) device;
};

SNOW_OWL_NAMESPACE_END
