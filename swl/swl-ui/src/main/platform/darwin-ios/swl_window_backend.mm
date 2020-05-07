//
//  swl_window_backend.m
//  ui_iOS
//
//  Created by Michael Ong on 6/5/20.
//  Copyright © 2020 mong Industries. All rights reserved.
//
#import "swl_window_backend.hpp"
#import <UIKit/UIKit.h>

SNOW_OWL_NAMESPACE(ui::backend)

window_backend *window_backend::instance = new window_backend();

void  window_backend::create        (window const *window) {

  native_handles.emplace(window, nullptr);

  if (native_handles.size() > 1) {
    NSUserActivity* activity          = [[NSUserActivity new] initWithActivityType:@"net.rantingmong.swl.activity"];
                    activity.userInfo = @{ @"window_handle" : @(window->get_handle()) };

    [[UIApplication sharedApplication] requestSceneSessionActivation:nil
                                                        userActivity:activity
                                                             options:nil
                                                        errorHandler:^(NSError * _Nonnull error) {

    }];
  }
}

void  window_backend::destroy       (window const *window) {

  void* pre_ref   = native_handles.at(window);
  auto  ns_window = (__bridge_transfer UIWindow*) pre_ref;

  native_handles.erase(window);

  (void) ns_window; // swl scope for window ends here

  // never delete the backend instance in iOS
}


void  window_backend::update_title  (window const *window) {

  dispatch_async(dispatch_get_main_queue(), ^{

    void* pre_ref   = native_handles.at(window);
    auto  ns_window = (__bridge UIWindow*) pre_ref;
  });
}

void  window_backend::update_frame  (window const *window) {

  dispatch_async(dispatch_get_main_queue(), ^{

    void* pre_ref   = native_handles.at(window);
    auto  ns_window = (__bridge UIWindow*) pre_ref;
  });
}


void* window_backend::surface       (window const *window) {

  void* pre_ref   = native_handles.at(window);
  auto  ui_window = (__bridge UIWindow*) pre_ref;

  return (__bridge void*) ui_window.rootViewController.view.layer;
}

cx::exp::ptr_ref<ui::window>
      window_backend::main_window   () {
  return cx::exp::ptr_ref<ui::window>{ nullptr };
}

SNOW_OWL_NAMESPACE_END
