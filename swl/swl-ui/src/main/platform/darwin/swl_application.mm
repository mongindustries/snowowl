#include "swl_application.hxx"
#include "application.hpp"

using namespace swl::ui;

SwlApplicationDelegate* app_delegate = [[SwlApplicationDelegate alloc] init];

@implementation SwlApplicationDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)notification {

}

@end

void Application::RunLoop(Application &app) {
	[[NSApplication sharedApplication] run];
}

void Application::PreHeat(Application &app) {
	[NSApplication sharedApplication];
	[NSApplication sharedApplication].delegate = app_delegate;
}
