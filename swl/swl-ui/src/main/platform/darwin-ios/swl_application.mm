//
//  swl_application.m
//  ui_iOS
//
//  Created by Michael Ong on 6/5/20.
//  Copyright © 2020 mong Industries. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <vector>

#import "swl_application.h"
#import "swl_scene.h"

#import <ownership.hpp>
#import "window.hpp"

using namespace swl;

ui::application* app_instance = nullptr;
swl_application* app_delegate = nullptr;

@implementation swl_application {

  cx::exp::ptr<ui::window>  __main_window;
}

+ (swl_application *)instance {
  return app_delegate;
}

- (BOOL)
  application:(UIApplication *)application
  didFinishLaunchingWithOptions:(NSDictionary<UIApplicationLaunchOptionsKey,id> *)launchOptions {

  // create initial scene
  app_delegate = self;


  // fire on create
  app_instance->on_create();

  return YES;
}

- (UISceneConfiguration *)
  application:(UIApplication *)application
  configurationForConnectingSceneSession:(UISceneSession *)connectingSceneSession
  options:(UISceneConnectionOptions *)options {

  UISceneConfiguration* config = [[UISceneConfiguration new] initWithName:@"SnowOwl: Scene" sessionRole:connectingSceneSession.role];

  config.delegateClass  = [swl_scene class];
  config.sceneClass     = [UIWindowScene class];

  return config;
}


- (void)
  applicationWillTerminate:(UIApplication *)application {
  app_instance->on_destroy();
}

- (cx::exp::ptr_ref<ui::window>) get_main_window {
  return cx::exp::ptr_ref<ui::window>{ __main_window };
}

@end

SNOW_OWL_NAMESPACE(ui)

void application::pre_heat  (application &app) {
  app_instance = &app;
}

int application::run_loop   (application &app) {

  auto process = [[NSProcessInfo processInfo] arguments];

  std::vector<char*> arg_v;
  arg_v.reserve(process.count);

  for (NSString* arg : process) {
    arg_v.emplace_back((char*) [arg cStringUsingEncoding:NSUTF8StringEncoding]);
  }

  return UIApplicationMain((int) process.count, arg_v.data(), nullptr, NSStringFromClass([swl_application class]));
}

cx::exp::ptr_ref<ui::window> application::get_main_window() {
  return app_delegate.main_window;
}

SNOW_OWL_NAMESPACE_END
