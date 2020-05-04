#import <tell.hpp>

#import "application.hpp"
#import "swl_application.hxx"

using namespace swl::ui;

application* __app = nullptr;

application_delegate* app_delegate = [[application_delegate alloc] init];

@implementation application_delegate

- (void)applicationDidFinishLaunching:  (NSNotification *)notification {
  __app->on_create  ();
}

- (void)applicationWillTerminate:       (NSNotification *)notification {
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
