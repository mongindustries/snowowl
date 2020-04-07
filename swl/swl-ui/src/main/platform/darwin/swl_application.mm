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
		app.delegate = app_delegate;
		app.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
	});
}
