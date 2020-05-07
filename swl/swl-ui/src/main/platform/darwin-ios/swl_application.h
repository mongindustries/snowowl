//
//  swl_application.h
//  ui_iOS
//
//  Created by Michael Ong on 6/5/20.
//  Copyright © 2020 mong Industries. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "application.hpp"

@interface swl_application : NSObject< UIApplicationDelegate >

@property swl::ui::application* app;

@property(readonly, getter=get_main_window) swl::cx::exp::ptr_ref<swl::ui::window> main_window;


+ (swl_application*) instance;

@end
