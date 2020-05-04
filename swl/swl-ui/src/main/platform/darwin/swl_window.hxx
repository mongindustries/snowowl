//
// Created by Michael Ong on 4/4/20.
//
#pragma once

#import <AppKit/AppKit.h>

#import <ownership.hpp>

#import "window.hpp"

#import "swl_window_sink.hpp"
#import "swl_viewController.hxx"

@interface ui_window: NSWindow

@property swl::cx::exp::ptr_ref<swl::ui::window> sink;

+ (ui_window*)createWindow: (ui_view_controller*) controller engineWindow:(const swl::ui::window*) window;

@end
