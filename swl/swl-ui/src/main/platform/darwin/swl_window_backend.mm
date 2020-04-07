//
// Created by Michael Ong on 4/4/20.
//
#import <tell.hpp>

#import "swl_window_backend.hpp"

#import "swl_window.hxx"
#import "swl_viewController.hxx"

#import "windowSurface.hpp"

using namespace std;

using namespace swl::ui;
using namespace swl::ui::backend;

using namespace swl::cx;


NSWindow* getWindowFromHandle(const map<reference_wrapper<Window>, void*, less<const Window>> &list, Window &check) {

	reference_wrapper<Window> windowKey { check };
	auto item = list.at(windowKey);

	return (__bridge NSWindow *) item;
}


void WindowBackend::Spawn
	(Window &window) {

	window._handle = Core::makeHandle();

	auto frame = window._frame;
	auto viewFrame = NSRect { { frame.origin.x, frame.origin.y }, { frame.size.x, frame.size.y } };

	auto controller = Tell<swlViewController>([[swlViewController alloc] initWithNibName:nil bundle:nil], [&window, viewFrame](swlViewController *ctx) {
		Tell<NSView>(ctx.view, [viewFrame](NSView *view) {
			view.frame = viewFrame;
		});
	});

	swlWindow* ns_window = [swlWindow createWindow:controller];

	ns_window.title   = [NSString stringWithCString:window._title.c_str() encoding:NSUTF8StringEncoding];
	ns_window.window  = &window;

	[ns_window makeKeyAndOrderFront: [NSApplication sharedApplication]];
	[ns_window makeMainWindow];

	activeNativeHandles.emplace(pair { reference_wrapper { window }, (__bridge_retained void*) ns_window });
}

void WindowBackend::Resized
	(Window &window, const cx::Rect &rect) {
	window._frame = rect;
}


void WindowBackend::UpdateTitle
	(Window const &window) {
	try {

		auto ns_window = getWindowFromHandle(activeNativeHandles, const_cast<Window&>(window));

		ns_window.contentViewController.title = [NSString stringWithCString:window._title.c_str() encoding:NSUTF8StringEncoding];
	} catch (const out_of_range&) {
	}
}

void WindowBackend::UpdateFrame
	(Window const &window) {
	try {

		auto ns_window = getWindowFromHandle(activeNativeHandles, const_cast<Window&>(window));

		auto frame = window._frame;
		[ns_window setFrame: NSMakeRect(frame.origin.x, frame.origin.y, frame.size.x, frame.size.y) display: YES];
	} catch (const out_of_range&) {
	}
}

WindowSurface WindowBackend::PrepareSurface
	(Window const &window) {

	try {

		auto ns_window = getWindowFromHandle(activeNativeHandles, const_cast<Window&>(window));

		auto surface = WindowSurface();

		surface._handle = Core::makeHandle();
		surface._native_surface_handle = (__bridge void*) ns_window.contentViewController.view.layer;

		surface._window = &window;

		return surface;
	} catch (const out_of_range&) {
		return WindowSurface();
	}
}


void WindowBackend::Close
	(Window const &window) {
	try {
		@autoreleasepool {

			decltype(activeNativeHandles)::key_type windowKey { const_cast<Window&>(window) };

			auto window_item = activeNativeHandles.at(windowKey);
			auto ns_window = (__bridge_transfer NSWindow *) window_item;

			for(const auto &event : window._event_close_list) {
				event(window);
			}

			(void) ns_window; // end scope of window here

			activeNativeHandles.erase(windowKey);

			auto &lis = application->_window_list;
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
