//
// Created by Michael Ong on 4/4/20.
//
#pragma once

#import <AppKit/AppKit.h>
#import "window.hpp"
#import "swl_viewController.hxx"

@interface swlWindow: NSWindow

@property(assign) swl::ui::Window *window;

+ (swlWindow*) createWindow: (swlViewController*) controller;

@end
