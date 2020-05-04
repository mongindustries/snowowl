//
//  context.m
//  app-macOS
//
//  Created by Michael Ong on 4/5/20.
//  Copyright © 2020 mong Industries. All rights reserved.
//

#import <Metal/Metal.h>

#import "metal/context.h"

SNOW_OWL_NAMESPACE(gx::mtl)

context::context(): device(MTLCreateSystemDefaultDevice()) { }

SNOW_OWL_NAMESPACE_END
