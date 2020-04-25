//
// Created by Michael Ong on 4/4/20.
//
#import <tell.hpp>

#import "swl_window_backend.hpp"

#import "swl_window.hxx"
#import "swl_viewController.hxx"

#import "window_surface.hpp"

using namespace std;

using namespace swl::ui;
using namespace swl::ui::backend;

using namespace swl::cx;

window_backend *window_backend::instance = new window_backend();


void  window_backend::create        (window const *window) {

	auto frame      = window->get_frame();
	auto viewFrame  = NSMakeRect(frame.origin.x(), frame.origin.y(), frame.size.x(), frame.size.y());

	auto controller = [[ui_view_controller alloc] initWithNibName:nil bundle:nil];
	[controller loadView];

	controller.view.frame = viewFrame;

	auto ns_window = [ui_window createWindow:controller engineWindow:window];

	[ns_window center];

	[ns_window makeKeyAndOrderFront: [NSApplication sharedApplication]];
	[ns_window makeMainWindow];

	native_handles.emplace(window, (__bridge_retained void*) ns_window);
}

void  window_backend::destroy       (window const *window) {

	void* pre_ref   = native_handles.at(window);
	auto  ns_window = (__bridge_transfer NSWindow*) pre_ref;

	native_handles.erase(window);

	(void) ns_window; // swl scope for window ends here

	if (native_handles.empty()) {
		delete window_backend::instance;

		[[NSApplication sharedApplication] terminate:nil];
	}
}


void  window_backend::update_title  (window const *window) {

	dispatch_async(dispatch_get_main_queue(), ^{

		void* pre_ref   = native_handles.at(window);
		auto  ns_window = (__bridge NSWindow*) pre_ref;

		[ns_window setTitle:[NSString stringWithCString:window->get_title().c_str() encoding:NSUTF8StringEncoding]];
	});
}

void  window_backend::update_frame  (window const *window) {

	dispatch_async(dispatch_get_main_queue(), ^{

		void* pre_ref   = native_handles.at(window);
		auto  ns_window = (__bridge NSWindow*) pre_ref;

		auto  frame     = window->get_frame();
		[ns_window.contentViewController.view setFrame: NSMakeRect(frame.origin.x(), frame.origin.y(), frame.size.x(), frame.size.y())];
	});
}


void* window_backend::surface       (window const *window) {

	void* pre_ref   = native_handles.at(window);
	auto  ns_window = (__bridge NSWindow*) pre_ref;

	return (__bridge void*) ns_window.contentViewController.view.layer;
}
