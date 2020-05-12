//
//  render_block.h
//  SnowOwl
//
//  Created by Michael Ong on 11/5/20.
//  Copyright © 2020 mong Industries. All rights reserved.
//
#pragma once

#import <header.hpp>

#import "render_block.hpp"

SNOW_OWL_NAMESPACE(gx::mtl)

struct queue;

struct render_pipeline;

struct render_block: gx::render_block {

  render_block  (mtl::queue& context, mtl::render_pipeline& pipeline);


  void reset    (gx::render_pipeline& pipeline) override;

  void close    () override;


  OBJC_ID(MTLRenderPipelineState) bounded_pipeline;
};

SNOW_OWL_NAMESPACE_END
