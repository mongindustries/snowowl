#import <tell.hpp>
#import <ownership.hpp>

#import "application.hpp"
#import "swl_application.hxx"

using namespace swl;
using namespace swl::ui;

application* __app = nullptr;

application_delegate* app_delegate = [[application_delegate alloc] init];

@implementation application_delegate {
  cx::exp::ptr<window> __main_window;
}

- (instancetype)init {

  if (self = [super init]) {
    __main_window = cx::exp::ptr<window>{ nullptr };
  }

  return self;
}

- (swl::cx::exp::ptr_ref<swl::ui::window>)
  get_window {
  if (__main_window) {
    return cx::exp::ptr_ref{ __main_window };
  }
  __main_window = cx::exp::ptr<ui::window>{ new ui::window{ "Main Window", cx::rect{{}, {400, 400}}} };
  return cx::exp::ptr_ref{__main_window};
}

- (void)
  applicationDidFinishLaunching:  (NSNotification *)notification {

  __app->on_create  ();
}

- (void)
  applicationWillTerminate:       (NSNotification *)notification {
  __app->on_destroy ();
}

@end


int   application::run_loop(application &app) {

  @autoreleasepool {
    [[NSApplication sharedApplication] run];
  }

  return 0;
}

void  application::pre_heat(application &app) {

  __app = &app;

  @autoreleasepool {

    cx::tell<NSApplication>([NSApplication sharedApplication], [](const NSApplication *app) {

      auto appMenu = [[NSMenuItem alloc] initWithTitle:@"Application" action:nil keyEquivalent:@""];

      auto appsubMenu = [[NSMenu alloc] init];
      appMenu.submenu = appsubMenu;

      auto serviceMenu = [[NSMenu alloc] init];
      app.servicesMenu = serviceMenu;

      [appsubMenu addItemWithTitle:@"About" action:nil keyEquivalent:@""];
      [appsubMenu addItem:NSMenuItem.separatorItem];

      [appsubMenu addItemWithTitle:@"Console" action:nil keyEquivalent:@""];
      [appsubMenu addItemWithTitle:@"Preferences" action:nil keyEquivalent:@""];
      [appsubMenu addItemWithTitle:@"Services" action:nil keyEquivalent:@""].submenu = serviceMenu;
      [appsubMenu addItem:NSMenuItem.separatorItem];

      auto hideMenu = [appsubMenu addItemWithTitle:@"Hide" action:@selector(hide:) keyEquivalent:@"h"];
      hideMenu.keyEquivalentModifierMask = NSEventModifierFlagCommand;

      auto hideAllMenu = [appsubMenu addItemWithTitle:@"Hide Others" action:@selector(hideOtherApplications:) keyEquivalent:@"h"];
      hideAllMenu.keyEquivalentModifierMask = NSEventModifierFlagCommand | NSEventModifierFlagShift;

      [appsubMenu addItem:NSMenuItem.separatorItem];
      [appsubMenu addItemWithTitle:@"Go Fullscreen" action:@selector(toggleFullScreen:) keyEquivalent:@""];
      [appsubMenu addItem:NSMenuItem.separatorItem];

      auto quitMenu = [appsubMenu addItemWithTitle:@"Quit" action:@selector(terminate:) keyEquivalent:@"q"];
      quitMenu.keyEquivalentModifierMask = NSEventModifierFlagCommand;

      auto mainMenu = [[NSMenu alloc] init];

      [mainMenu addItem:appMenu];

      app.mainMenu = mainMenu;
      app.delegate = app_delegate;

      app.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    });
  }
}

cx::exp::ptr_ref<window>
      application::get_main_window() {
  return [app_delegate get_window];
}
