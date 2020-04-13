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

@interface swlWindow(Delegate)<NSWindowDelegate>

@end

@implementation swlWindow

- (void)windowDidBecomeKey: (NSNotification*) notification {
	if (_window != nullptr) {
	}
}

- (void)windowDidResignKey: (NSNotification*) notification {
	if (_window != nullptr) {
	}
}

- (void)windowWillClose: (NSNotification*) notification {
	if (_window != nullptr) {
		_window->Closed();
	}
}

- (void)windowDidResize: (NSNotification*) notification {
	if (_window != nullptr) {

		auto frame = self.frame;

		_window->Update(swl::cx::Rect{
			.origin = { (float)frame.origin.x, (float)frame.origin.y },
			.size   = {   (int)frame.size.width, (int)frame.size.height } });
	}
}

+ (swlWindow *)createWindow:(swlViewController *)controller engineWindow:(const Window *)window {

	auto masks = NSWindowStyleMaskClosable |
	             NSWindowStyleMaskMiniaturizable |
	             NSWindowStyleMaskFullSizeContentView |
	             NSWindowStyleMaskResizable |
	             NSWindowStyleMaskClosable |
	             NSWindowStyleMaskTitled;

	auto screen   = [NSScreen mainScreen];
	auto instance = [[swlWindow alloc] initWithContentRect:CGRectZero styleMask:masks backing:NSBackingStoreBuffered defer:NO screen:screen];

	return Tell<swlWindow>(instance, [&window, &controller](swlWindow *ctx) {

		ctx.title                 = [NSString stringWithCString:window->getTitle().c_str() encoding:NSUTF8StringEncoding];

		ctx.delegate              = ctx;
		ctx.contentViewController = controller;

		ctx.window                = window->getSink();

		ctx.minSize               = NSMakeSize(400, 400);
	});
}

@end
