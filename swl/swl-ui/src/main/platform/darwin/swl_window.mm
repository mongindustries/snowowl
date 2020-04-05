//
// Created by Michael Ong on 4/4/20.
//
#import <tell.hpp>

#import "swl_window.hxx"
#import "swl_viewController.hxx"

#import "swl_window_backend.hpp"

using namespace swl::cx;

using namespace swl::ui;
using namespace swl::ui::backend;

@interface swlWindow(Delegate)<NSWindowDelegate>

@end

@implementation swlWindow

- (void)windowWillClose: (NSNotification*) notification {
	if (_window != nullptr) {
		WindowBackend::backend->Close(_window);
		_window = nullptr;
	}
}

- (void)windowDidResize: (NSNotification*) notification {
	if (_window != nullptr) {

		auto frame = self.frame;
		auto wRect = swl::cx::Rect {
			{ (float) frame.origin.x, (float) frame.origin.y },
			{ (float) frame.size.width, (float) frame.size.height }
		};

		WindowBackend::backend->Resized(_window, wRect);
	}
}

+ (swlWindow *)createWindow:(swlViewController *) controller {

	auto masks = NSWindowStyleMaskClosable |
	             NSWindowStyleMaskMiniaturizable |
	             NSWindowStyleMaskFullSizeContentView |
	             NSWindowStyleMaskResizable |
	             NSWindowStyleMaskClosable |
	             NSWindowStyleMaskTitled;
	auto screen = [NSScreen mainScreen];
	auto window = [[swlWindow alloc] initWithContentRect:CGRectZero styleMask:masks backing:NSBackingStoreBuffered defer:NO screen:screen];

	return tell<swlWindow>(window, [&controller](swlWindow *ctx) {
		ctx.delegate = ctx;

		ctx.contentViewController = controller;

		ctx.titlebarAppearsTransparent = YES;
		ctx.titleVisibility = NSWindowTitleVisible;
	});
}

@end
