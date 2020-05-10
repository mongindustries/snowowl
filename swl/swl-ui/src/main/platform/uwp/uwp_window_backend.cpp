#include "swl_window_backend.hpp"
#include "swl_window_sink.hpp"

#include <string>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <unknwn.h>
#include <winrt/base.h>

#include <winrt/windows.applicationmodel.core.h>
#include <winrt/windows.ui.core.h>
#include <winrt/windows.ui.viewmanagement.h>

#include <swap_chain.hpp>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::UI::Core;
using namespace Windows::UI;

SNOW_OWL_NAMESPACE(ui)

struct window_invalid_call: std::exception { };

backend::window_backend* backend::window_backend::instance = new backend::window_backend();

std::mutex proc_mutex;

void  backend::window_backend::create         (window const* window) {
  throw window_invalid_call();
}

void  backend::window_backend::create_native  (window const* window, void* native) {

  CoreWindow core_window{ nullptr };
  ((::IUnknown*) native)->QueryInterface(winrt::guid_of<CoreWindow>(), winrt::put_abi(core_window));

  native_handles.emplace(window, native);

  if (auto view = Windows::UI::ViewManagement::ApplicationView::GetForCurrentView()) {
    view.SetPreferredMinSize({ 400, 400 });

    auto& title_bar = view.TitleBar();

    title_bar.ButtonBackgroundColor (Colors::Transparent());
    title_bar.BackgroundColor       (Colors::Transparent());
  }

  update_title(window);

  if (window->get_size().components == window::fullscreen_size.components) {
    const_cast<ui::window*>(window)->set_fullscreen(true);
  }

  core_window.ResizeStarted   ([window](CoreWindow const&, IInspectable const&) {
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

  core_window.ResizeCompleted ([window](CoreWindow const& cw, IInspectable const&) {

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

  core_window.SizeChanged     ([window](CoreWindow const& cw, WindowSizeChangedEventArgs const& args) {

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

  core_window.Closed          ([window](CoreWindow const&, CoreWindowEventArgs const& args) {

    if (window->game_loop) {

      // wait game loop frame complete...
      std::unique_lock<std::mutex> lock{ proc_mutex };
      window->game_loop->target_lock.wait(lock);
      window->game_loop->close();
    }

    window->get_sink()->closed();

    args.Handled(true);
  });
}

void  backend::window_backend::destroy        (window const* window) {
}

void  backend::window_backend::fullscreen     (window const* window) {

  auto handle = native_handles.find(window);

  if (handle == native_handles.end()) {
    return;
  }

  CoreWindow core_window{ nullptr };
  ((::IUnknown*) handle->second)->QueryInterface(winrt::guid_of<CoreWindow>(), winrt::put_abi(core_window));

  core_window.Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, [window]() {

    auto& view = Windows::UI::ViewManagement::ApplicationView::GetForCurrentView();

    if (window->get_fullscreen()) {
      view.TryEnterFullScreenMode();
    } else {
      view.ExitFullScreenMode();
    }
  });
}


void  backend::window_backend::update_title   (window const* window) {

  auto handle = native_handles.find(window);

  if (handle == native_handles.end()) {
    return;
  }

  CoreWindow core_window{ nullptr };
  ((::IUnknown*) handle->second)->QueryInterface(winrt::guid_of<CoreWindow>(), winrt::put_abi(core_window));

  core_window.Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, [window]() {

    auto& view = Windows::UI::ViewManagement::ApplicationView::GetForCurrentView();

    if (!view) {
      return;
    }


    auto &bstring = window->get_title();
    auto wstrings = MultiByteToWideChar(CP_UTF8, 0, bstring.c_str(), -1, nullptr, 0);

    std::wstring title;
    title.reserve(wstrings);

    MultiByteToWideChar(CP_UTF8, 0, bstring.c_str(), -1, title.data(), wstrings);

    view.Title(title.c_str());
  });
}

void  backend::window_backend::update_frame   (window const* window) {

  auto handle = native_handles.find(window);

  if (handle == native_handles.end()) {
    return;
  }

  CoreWindow core_window{ nullptr };
  ((::IUnknown*) handle->second)->QueryInterface(winrt::guid_of<CoreWindow>(), winrt::put_abi(core_window));

  core_window.Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, [window]() {

    auto& view = Windows::UI::ViewManagement::ApplicationView::GetForCurrentView();

    if (!view) {
      return;
    }

    auto& size = window->get_size();
    view.TryResizeView({ (float) size.x(), (float) size.y() });
  });
}


void* backend::window_backend::surface        (window const* window) {
  auto handle = native_handles.find(window);

  if (handle == native_handles.end()) {
    return nullptr;
  }

  return handle->second;
}

SNOW_OWL_NAMESPACE_END
