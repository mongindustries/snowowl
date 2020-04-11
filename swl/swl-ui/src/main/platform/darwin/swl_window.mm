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

- (void)windowDidBecomeKey: (NSNotification*) notification {
	if (_window != nullptr) {
		for (const auto &item : _window->_event_state_list) {
			item.invoke(*_window, Window::State::Active);
		}
	}
}

- (void)windowDidResignKey: (NSNotification*) notification {
	if (_window != nullptr) {
		for (const auto &item : _window->_event_state_list) {
			item.invoke(*_window, Window::State::Background);
		}
	}
}

- (void)windowWillClose: (NSNotification*) notification {
	if (_window != nullptr) {
		WindowBackend::backend->Close(*_window);
		_window = nullptr;
	}
}

- (void)windowDidResize: (NSNotification*) notification {
	if (_window != nullptr) {

		auto frame = self.frame;
		auto wRect = swl::cx::Rect {
			{ (float) frame.origin.x, (float) frame.origin.y },
			{ (int) frame.size.width, (int) frame.size.height }
		};

		auto& ref_window = *_window;
		WindowBackend::backend->Resized(ref_window, wRect);
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

	return Tell<swlWindow>(window, [&controller](swlWindow *ctx) {
		ctx.delegate = ctx;

		ctx.contentViewController = controller;
		ctx.minSize = NSMakeSize(400, 400);
	});
}

@end
