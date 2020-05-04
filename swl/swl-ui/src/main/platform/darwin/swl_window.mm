//
// Created by Michael Ong on 4/4/20.
//
#import <tell.hpp>
#import <rect.hpp>

#import "swl_window.hxx"
#import "swl_window_backend.hpp"

#import <swap_chain.hpp>

using namespace swl;

@interface ui_window(Delegate)<NSWindowDelegate>

@end

@implementation ui_window {

  std::mutex target_lock;
}

- (void)windowDidBecomeKey: (NSNotification*) notification {
  if (_sink) {
  }
}

- (void)windowDidResignKey: (NSNotification*) notification {
  if (_sink) {
  }
}

- (void)windowWillClose: (NSNotification*) notification {
  if (_sink) {
    _sink->get_sink()->closed();
  }
}


- (void)windowWillStartLiveResize: (NSNotification*) notification {
  if (_sink) {
    _sink->get_sink()->sizing(true);

    if (_sink->game_loop) {

      _sink->game_loop->check_for_lock = true;

      std::unique_lock<std::mutex> lock{ target_lock  };
      _sink->game_loop->target_lock.wait(lock);
    }

    if (_sink->swap_chain) {
      _sink->swap_chain->swaps_immediately = true;
    }
  }
}

- (void)windowDidEndLiveResize: (NSNotification*) notification {
  if (_sink) {

    auto frame = self.frame;

    _sink->get_sink()->frame(swl::cx::rect{
      { (float) frame.origin.x, (float) frame.origin.y },
      { (int) frame.size.width, (int) frame.size.height } });

    // resizing the swapchain one last time
    if (_sink->swap_chain) {
      _sink->swap_chain->resize(_sink->get_size());
      _sink->swap_chain->swaps_immediately = false;
    }

    // then run a game loop frame
    if (_sink->game_loop) {
      _sink->game_loop->frame();

      // then give back execution of the frame back to the game loop
      _sink->game_loop->check_for_lock = false;
      _sink->game_loop->loop_lock.notify_all();
    }


    _sink->get_sink()->sizing(false);
  }
}

- (void)windowDidResize: (NSNotification*) notification {
  if (_sink) {

    auto frame = self.frame;

    // this flag is for when the window is interactively resized, that is,
    // there is a resize drag going on.
    // the execution order is different since the frame locks are done at
    // the interactive resize begin/end.
    bool interactive_resize = _sink->is_sizing();

    _sink->get_sink()->frame(swl::cx::rect{
      { (float) frame.origin.x, (float) frame.origin.y },
      { (int) frame.size.width, (int) frame.size.height } });

    // if this window is not interactively resizing (maximize, restore operations)
    if (!interactive_resize) {

      if (_sink->game_loop) {

        // ...so that we can lock its execution in-order
        _sink->game_loop->check_for_lock = true;

        // wait game loop frame complete...
        std::unique_lock<std::mutex> lock{ target_lock };
        _sink->game_loop->target_lock.wait(lock);
      }
    }

    // this thread now owns the framing...
    if (_sink->swap_chain) {
      // ...start by resizing the swapchain
      _sink->swap_chain->resize(_sink->get_size());
    }

    // ...then run a game loop frame
    if (_sink->game_loop) {
      _sink->game_loop->frame();

      // ...then give back execution of the frame back to the game loop
      if (!interactive_resize) {
        _sink->game_loop->check_for_lock = false;
        _sink->game_loop->loop_lock.notify_all();
      }
    }
  }
}

+ (ui_window *)createWindow:(ui_view_controller *)controller engineWindow:(const ui::window *)window {

  auto masks = NSWindowStyleMaskClosable |
               NSWindowStyleMaskMiniaturizable |
               NSWindowStyleMaskFullSizeContentView |
               NSWindowStyleMaskResizable |
               NSWindowStyleMaskClosable |
               NSWindowStyleMaskTitled;

  auto screen   = [NSScreen mainScreen];
  auto instance = [[ui_window alloc] initWithContentRect:CGRectZero styleMask:masks backing:NSBackingStoreBuffered defer:NO screen:screen];

  return cx::tell<ui_window>(instance, [&window, &controller](ui_window *ctx) {

    ctx.title = [NSString stringWithCString:window->get_title().c_str() encoding:NSUTF8StringEncoding];

    ctx.delegate = ctx;
    ctx.contentViewController = controller;

    ctx.titlebarAppearsTransparent  = YES;
    ctx.titleVisibility             = NSWindowTitleHidden;

    ctx.sink    = cx::exp::ptr_ref{ const_cast<ui::window*>(window) };

    ctx.minSize = NSMakeSize(400, 400);
  });
}

@end
