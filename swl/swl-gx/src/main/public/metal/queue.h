//
//  queue.h
//  SnowOwl
//
//  Created by Michael Ong on 4/5/20.
//  Copyright © 2020 mong Industries. All rights reserved.
//
#pragma once

#include <header.hpp>

#include "queue.hpp"

SNOW_OWL_NAMESPACE(gx::mtl)

struct queue: gx::queue {

  explicit queue  (gx::context& context);


  void
    begin         (std::vector<cx::exp::ptr_ref<gx::queue>> const& dependencies) override;

  void
    submit        (std::vector<cx::exp::ptr_ref<gx::render_block>> const& commands) override;


  OBJC_ID(MTLCommandQueue)  command_queue;

  OBJC_ID(MTLCommandBuffer) current_buffer;
};

SNOW_OWL_NAMESPACE_END
