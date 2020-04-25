//
// Created by Michael Ong on 4/4/20.
//
#import <tell.hpp>
#import <rect.hpp>

#import "swl_window.hxx"
#import "swl_window_backend.hpp"

using namespace swl::cx;

using namespace swl::ui;
using namespace swl::ui::backend;

@interface ui_window(Delegate)<NSWindowDelegate>

@end

@implementation ui_window

- (void)windowDidBecomeKey: (NSNotification*) notification {
	if (_sink) {
	}
}

- (void)windowDidResignKey: (NSNotification*) notification {
	if (_sink) {
	}
}

- (void)windowWillClose: (NSNotification*) notification {
	if (_sink) {
		_sink->closed();
	}
}


- (void)windowWillStartLiveResize: (NSNotification*) notification {
	if (_sink) {
		_sink->sizing(true);
	}
}

- (void)windowDidEndLiveResize: (NSNotification*) notification {
	if (_sink) {
		_sink->sizing(false);
	}
}

- (void)windowDidResize: (NSNotification*) notification {
	if (_sink) {

		auto frame = self.frame;

		_sink->frame(swl::cx::rect{
			{ (float) frame.origin.x, (float) frame.origin.y },
			{ (int) frame.size.width, (int) frame.size.height } });
	}
}

+ (ui_window *)createWindow:(ui_view_controller *)controller engineWindow:(const window *)window {

	auto masks = NSWindowStyleMaskClosable |
	             NSWindowStyleMaskMiniaturizable |
	             NSWindowStyleMaskFullSizeContentView |
	             NSWindowStyleMaskResizable |
	             NSWindowStyleMaskClosable |
	             NSWindowStyleMaskTitled;

	auto screen   = [NSScreen mainScreen];
	auto instance = [[ui_window alloc] initWithContentRect:CGRectZero styleMask:masks backing:NSBackingStoreBuffered defer:NO screen:screen];

	return tell<ui_window>(instance, [&window, &controller](ui_window *ctx) {

		ctx.title = [NSString stringWithCString:window->get_title().c_str() encoding:NSUTF8StringEncoding];

		ctx.delegate = ctx;
		ctx.contentViewController = controller;

		ctx.titlebarAppearsTransparent  = YES;
		ctx.titleVisibility             = NSWindowTitleHidden;

		ctx.sink    = window->get_sink();

		ctx.minSize = NSMakeSize(400, 400);
	});
}

@end
