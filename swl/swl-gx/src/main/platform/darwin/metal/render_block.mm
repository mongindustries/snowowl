//
//  render_block.m
//  gx_macOS
//
//  Created by Michael Ong on 11/5/20.
//  Copyright © 2020 mong Industries. All rights reserved.
//

#import <Metal/Metal.h>

#import "metal/queue.h"
#import "metal/render_block.h"
#import "metal/render_pipeline.h"

SNOW_OWL_NAMESPACE(gx::mtl)

render_block::render_block  (mtl::queue &queue, mtl::render_pipeline &pipeline):
  gx::render_block(queue, pipeline), bounded_pipeline(pipeline.pipeline) {
}

void
  render_block::reset       (gx::render_pipeline &pipeline) {
  @autoreleasepool {
    bounded_pipeline = dynamic_cast<mtl::render_pipeline&>(pipeline).pipeline;
  }
}

void
  render_block::close       () {
  @autoreleasepool {
    bounded_pipeline = nil;
  }
}

SNOW_OWL_NAMESPACE_END
