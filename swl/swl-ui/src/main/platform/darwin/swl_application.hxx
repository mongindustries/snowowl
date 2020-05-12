//
// Created by Michael Ong on 4/4/20.
//
#pragma once

#import <AppKit/AppKit.h>

#import <ownership.hpp>
#import "window.hpp"

@interface application_delegate: NSObject<NSApplicationDelegate>

- (swl::cx::exp::ptr_ref<swl::ui::window>) get_window;

@end
