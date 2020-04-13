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

WindowBackend *WindowBackend::backend = new WindowBackend();


void WindowBackend::Spawn
	(const Window *window) {

	auto frame      = window->getFrame();
	auto viewFrame  = NSMakeRect(frame.origin.x(), frame.origin.y(), frame.size.x(), frame.size.y());

	auto controller = [[swlViewController alloc] initWithNibName:nil bundle:nil];
	[controller loadView];

	controller.view.frame = viewFrame;

	auto ns_window = [swlWindow createWindow:controller engineWindow:window];

	[ns_window makeKeyAndOrderFront: [NSApplication sharedApplication]];
	[ns_window makeMainWindow];

	activeNativeHandles.emplace(window, (__bridge_retained void*) ns_window);
}


void WindowBackend::UpdateTitle
	(Window const *window) {

	dispatch_async(dispatch_get_main_queue(), ^{

		void* pre_ref   = activeNativeHandles.at(window);
		auto  ns_window = (__bridge NSWindow*) pre_ref;

		[ns_window setTitle:[NSString stringWithCString:window->getTitle().c_str() encoding:NSUTF8StringEncoding]];
	});
}

void WindowBackend::UpdateFrame
	(Window const *window) {

	dispatch_async(dispatch_get_main_queue(), ^{

		void* pre_ref   = activeNativeHandles.at(window);
		auto  ns_window = (__bridge NSWindow*) pre_ref;

		auto  frame     = window->getFrame();
		[ns_window.contentViewController.view setFrame: NSMakeRect(frame.origin.x(), frame.origin.y(), frame.size.x(), frame.size.y())];
	});
}


void *WindowBackend::NativeSurface
	(Window const *window) {

	void* pre_ref   = activeNativeHandles.at(window);
	auto  ns_window = (__bridge NSWindow*) pre_ref;

	return (__bridge void*) ns_window.contentViewController.view.layer;
}


void WindowBackend::RemoveEntry
	(Window const *window) {

	void* pre_ref   = activeNativeHandles.at(window);
	auto  ns_window = (__bridge_transfer NSWindow*) pre_ref;

	activeNativeHandles.erase(window);

	(void) ns_window; // swl scope for window ends here

	if (activeNativeHandles.empty()) {
		delete WindowBackend::backend;

		[[NSApplication sharedApplication] terminate:nil];
	}
}
