//
// Created by Michael Ong on 4/4/20.
//
#pragma once

#import <AppKit/AppKit.h>

#import "window.hpp"

#import "swl_window_sink.hpp"
#import "swl_viewController.hxx"

@interface swlWindow: NSWindow

@property(assign) swl::ui::WindowSink *window;

+ (swlWindow*) createWindow: (swlViewController*) controller engineWindow:(const swl::ui::Window*) window;

@end
