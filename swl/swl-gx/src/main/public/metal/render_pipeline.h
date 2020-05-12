//
//  render_pipeline.h
//  gx_macOS
//
//  Created by Michael Ong on 11/5/20.
//  Copyright © 2020 mong Industries. All rights reserved.
//

#import <header.hpp>
#import "render_pipeline.hpp"

SNOW_OWL_NAMESPACE(gx::mtl)

struct context;

struct render_pipeline: gx::render_pipeline {

  render_pipeline (context& context);

  void construct  () override;


  OBJC_ID(MTLDevice)              device;
  OBJC_ID(MTLRenderPipelineState) pipeline;
};

SNOW_OWL_NAMESPACE_END
