//
//  queue.m
//  app-macOS
//
//  Created by Michael Ong on 4/5/20.
//  Copyright © 2020 mong Industries. All rights reserved.
//

#import <Metal/Metal.h>

#import "metal/context.h"
#import "metal/queue.h"

SNOW_OWL_NAMESPACE(gx::mtl)

queue::queue      (gx::context& context): gx::queue(context), command_queue([static_cast<mtl::context&>(context).device newCommandQueue]) {
}

void
  queue::begin    (std::vector<cx::exp::ptr_ref<gx::queue>> const& dependencies) {

    for (auto &item: dependencies) {
      [static_cast<queue*>(item.pointer())->current_buffer waitUntilCompleted];
    }

    [current_buffer waitUntilCompleted]; // TODO will change to an actual fence?
    current_buffer = [command_queue commandBuffer];
  }

void
  queue::submit   (std::vector<cx::exp::ptr_ref<gx::render_block>> const& commands) {
    [current_buffer commit];
  }

void
  queue::transfer (std::vector<cx::exp::ptr_ref<gx::buffer_staging>> const& bufers) {

    id<MTLBlitCommandEncoder> transferEncoder = [current_buffer blitCommandEncoder];

    [transferEncoder endEncoding];
    [current_buffer commit];
  }

SNOW_OWL_NAMESPACE_END
