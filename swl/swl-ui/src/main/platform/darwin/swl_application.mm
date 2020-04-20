#import <tell.hpp>

#include "swl_application.hxx"
#include "application.hpp"

using namespace swl::ui;

Application* __app = nullptr;

SwlApplicationDelegate* app_delegate = [[SwlApplicationDelegate alloc] init];

@implementation SwlApplicationDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)notification {
}

- (void)applicationWillTerminate:(NSNotification *)notification {
	__app->applicationDestroy();
}

@end

void Application::runLoop(Application &app) {

	[[NSApplication sharedApplication] run];
}

void Application::preHeat(Application &app) {

	__app = &app;

	cx::Tell<NSApplication>([NSApplication sharedApplication], [](const NSApplication* app) {

			auto appMenu = [[NSMenuItem alloc] initWithTitle:@"Application" action:nil keyEquivalent:@""];

			auto appsubMenu = [[NSMenu alloc] init];
			appMenu.submenu = appsubMenu;

			auto serviceMenu = [[NSMenu alloc] init];
			NSApp.servicesMenu = serviceMenu;

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
