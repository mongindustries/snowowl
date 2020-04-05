#import "swl_viewController.hxx"
#import "swl_view.hxx"

@implementation swlViewController

- (void)loadView {
	self.view = [[swlView alloc] initWithFrame:CGRectZero];
}

@end
