//
//  swl_scene.m
//  ui_iOS
//
//  Created by Michael Ong on 6/5/20.
//  Copyright © 2020 mong Industries. All rights reserved.
//

#import "swl_scene.h"
#import "swl_window_backend.hpp"

using namespace swl;

@implementation swl_scene {
  UIWindow* _scene_window;
}

- (void)scene:(UIScene *)scene willConnectToSession:(UISceneSession *)session options:(UISceneConnectionOptions *)connectionOptions {

  auto window_scene = (UIWindowScene*) scene;

  if (!window_scene) {
    return;
  }

  auto& handles = ui::backend::window_backend::instance->native_handles;

  if (handles.size() > 1) {

    NSUserActivity* act;

    for (NSUserActivity* item in connectionOptions.userActivities.allObjects) {

      if ([item.activityType isEqualToString: @"net.rantingmong.swl.activity"]) {
        act = item;
        break;
      }
    }

    auto  handle  = ((NSNumber*) [act.userInfo objectForKey:@"window_handle"]).longValue;
    for (auto& pair : handles) {

      if (pair.first->get_handle() == handle) {
        pair.second = (__bridge void*) self;
        break;
      }
    }
  } else {
    handles.begin()->second = (__bridge void*)self;
  }

  _scene_window             = [[UIWindow new ] initWithFrame:window_scene.coordinateSpace.bounds];
  _scene_window.windowScene = window_scene;

  [_scene_window makeKeyAndVisible];
}

@end
