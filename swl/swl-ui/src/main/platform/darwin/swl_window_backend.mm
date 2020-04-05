//
// Created by Michael Ong on 4/4/20.
//

#import "swl_window_backend.hpp"

#import "swl_window.hxx"
#import "swl_viewController.hxx"

#import <tell.hpp>

using namespace std;

using namespace swl::cx;

using namespace swl::ui;
using namespace swl::ui::backend;

void WindowBackend::Spawn(Window *window) {

	window->handle = Core::makeHandle();

	auto frame = window->frame;
	auto viewFrame = NSRect { { frame.origin.x, frame.origin.y }, { frame.size.x, frame.size.y } };

	auto controller = tell<swlViewController>([[swlViewController alloc] initWithNibName:nil bundle:nil], [&window, viewFrame](swlViewController *ctx) {
		tell<NSView>(ctx.view, [viewFrame](NSView *view) {
			view.wantsLayer = YES;
			view.frame = viewFrame;

			view.layer.backgroundColor = [NSColor whiteColor].CGColor;
		});
	});

	swlWindow* ns_window = [swlWindow createWindow:controller];
	ns_window.title = [NSString stringWithCString:window->title.c_str() encoding:NSUTF8StringEncoding];
	ns_window.window = window;

	[ns_window makeKeyAndOrderFront: [NSApplication sharedApplication]];
	[ns_window makeMainWindow];

	activeNativeHandles.insert({ window->handle, (__bridge_retained void*) ns_window });
}


void WindowBackend::UpdateTitle(Window const* window) {
	try {
		auto window_item = activeNativeHandles.at(window->handle);
		auto ns_window = (__bridge NSWindow *) window_item;

		ns_window.contentViewController.title = [NSString stringWithCString:window->title.c_str() encoding:NSUTF8StringEncoding];
	} catch (const out_of_range&) {
	}
}

void WindowBackend::UpdateFrame(const swl::ui::Window *window) {
	try {
		auto window_item = activeNativeHandles.at(window->handle);
		auto ns_window = (__bridge NSWindow *) window_item;

		auto frame = window->frame;
		[ns_window setFrame: NSMakeRect(frame.origin.x, frame.origin.y, frame.size.x, frame.size.y) display: YES];
	} catch (const out_of_range&) {
	}
}


void WindowBackend::Close(const swl::ui::Window *window) {
	try {
		@autoreleasepool {
			auto window_item = activeNativeHandles.at(window->handle);
			auto ns_window = (__bridge_transfer NSWindow *) window_item;

			for_each(window->event_closeList.begin(), window->event_closeList.end(), [&window](const auto &event) {
				event(*window);
			});

			(void) ns_window; // end scope of window here

			activeNativeHandles.erase(window->handle);

			auto &lis = application->windowList;
			auto iter = find(lis.begin(), lis.end(), window);

			if (iter != lis.end()) {
				lis.erase(iter);
			}

			if (activeNativeHandles.empty()) {
				[[NSApplication sharedApplication] terminate: nil];
			}
		}
	} catch(const out_of_range&) {
	}
}

void WindowBackend::Resized(swl::ui::Window *window, const cx::Rect &rect) {
	window->frame = rect;
}


windowSurface WindowBackend::Surface(const Window *window) {
	return { Core::makeHandle() };
}
