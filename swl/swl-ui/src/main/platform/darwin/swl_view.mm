//
// Created by Michael Ong on 5/4/20.
//
#import "swl_view.hxx"

#import <tell.hpp>

#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>

@implementation swlView

- (instancetype)initWithFrame:(NSRect)frame {

	if (self = [super initWithFrame:frame]) {
		self.wantsLayer = YES;
	}

	return self;
}

- (instancetype)initWithCoder:(NSCoder*)codeable {
	if (self = [super initWithCoder:codeable]) {
		self.wantsLayer = YES;
	}

	return self;
}

- (CALayer *)makeBackingLayer {
	return swl::cx::tell<CAMetalLayer>([[CAMetalLayer alloc] init], [](CAMetalLayer *layer) {
			layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
	});
}

@end
