#include "swl_window_backend.hpp"
#include "swl_window_sink.hpp"

#include <Unknwn.h>
#include <winrt/base.h>

#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.UI.Core.h>

#include <swap_chain.hpp>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::UI::Core;

SNOW_OWL_NAMESPACE(ui)

backend::window_backend* backend::window_backend::instance = new backend::window_backend();

std::mutex proc_mutex;

void  backend::window_backend::create         (window const* window) {
  // fatal error, we can't create additional windows...
}

void  backend::window_backend::create_native  (window const* window, void* native) {

  CoreWindow core_window{ nullptr };
  ((::IUnknown*) native)->QueryInterface(winrt::guid_of<CoreWindow>(), winrt::put_abi(core_window));

  core_window.ResizeStarted([window](CoreWindow const&, IInspectable const&) {
    window->get_sink()->sizing(true);

    if (window->game_loop) {

      // ...so that we can lock its execution in-order
      window->game_loop->check_for_lock = true;

      // wait game loop frame complete...
      std::unique_lock<std::mutex> lock{ proc_mutex };
      window->game_loop->target_lock.wait(lock);
    }

    if (window->swap_chain) {
      window->swap_chain->swaps_immediately = true;
    }
  });

  core_window.ResizeCompleted([window](CoreWindow const& cw, IInspectable const&) {

    auto bounds = cw.Bounds();
    window->get_sink()->frame(cx::rect{ {bounds.X, bounds.Y}, {(int)bounds.Width, (int)bounds.Height} });

    // resizing the swapchain one last time
    if (window->swap_chain) {
      window->swap_chain->resize(window->get_size());
      window->swap_chain->swaps_immediately = false;
    }

    // then run a game loop frame
    if (window->game_loop) {
      window->game_loop->frame();

      // then give back execution of the frame back to the game loop
      window->game_loop->check_for_lock = false;
      window->game_loop->loop_lock.notify_all();
    }

    window->get_sink()->sizing(false);
  });

  core_window.SizeChanged([window](CoreWindow const& cw, WindowSizeChangedEventArgs const& args) {

    // this flag is for when the window is interactively resized, that is, 
      // there is a resize drag going on.
      // the execution order is different since the frame locks are done at
      // the interactive resize begin/end.
    bool interactive_resize = window->is_sizing();

    auto bounds = cw.Bounds();
    window->get_sink()->frame(cx::rect{ {bounds.X, bounds.Y}, {(int)bounds.Width, (int)bounds.Height} });

    // if this window is not interactively resizing (maximize, restore operations)
    if (!interactive_resize) {

      if (window->game_loop) {

        // ...so that we can lock its execution in-order
        window->game_loop->check_for_lock = true;

        // wait game loop frame complete...
        std::unique_lock<std::mutex> lock{ proc_mutex };
        window->game_loop->target_lock.wait(lock);
      }
    }

    // this thread now owns the framing...
    if (window->swap_chain) {
      // ...start by resizing the swapchain
      window->swap_chain->resize(window->get_size());
    }

    // ...then run a game loop frame
    if (window->game_loop) {
      window->game_loop->frame();

      // ...then give back execution of the frame back to the game loop
      if (!interactive_resize) {
        window->game_loop->check_for_lock = false;
        window->game_loop->loop_lock.notify_all();
      }
    }
  });

  native_handles.emplace(window, native);
}

void  backend::window_backend::destroy        (window const* window) {
}

void  backend::window_backend::fullscreen     (window const* window) {
}


void  backend::window_backend::update_title   (window const* window) {
  // does nothing
}

void  backend::window_backend::update_frame   (window const* window) {
  // does nothing
}


void* backend::window_backend::surface        (window const* window) {
  auto handle = native_handles.find(window);

  if (handle == native_handles.end()) {
    return nullptr;
  }

  return handle->second;
}

SNOW_OWL_NAMESPACE_END
