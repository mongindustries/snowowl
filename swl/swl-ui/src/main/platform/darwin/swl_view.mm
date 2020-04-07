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
		self.layer.backgroundColor = [NSColor whiteColor].CGColor;
	}

	return self;
}

- (instancetype)initWithCoder:(NSCoder*)coder {
	if (self = [super initWithCoder:coder]) {
		self.wantsLayer = YES;
	}

	return self;
}

- (CALayer *)makeBackingLayer {
	return swl::cx::Tell<CAMetalLayer>([[CAMetalLayer alloc] init], [](CAMetalLayer *layer) {
			layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
	});
}

@end
