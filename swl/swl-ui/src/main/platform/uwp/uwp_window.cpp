#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Unknwn.h>

#include "window.hpp"
#include "uwp_internal_state.h"

#include <winrt/Windows.UI.ViewManagement.Core.h>

using namespace Windows::UI;
using namespace Windows::UI::Core;

using namespace Windows::Foundation;

SNOW_OWL_NAMESPACE(ui)

window::window(std::nullptr_t) { }

window::window(const std::string &window_name, cx::rect const &frame, void *instance)
  : _state(new internal_state())
  , _handle(cx::core::make_handle()) {

  CoreWindow core_window{ nullptr };
  static_cast<::IUnknown*>(instance)->QueryInterface(winrt::guid_of<CoreWindow>(), put_abi(core_window));

  _state->core_window = make_agile(core_window);

  if (const auto view = ViewManagement::ApplicationView::GetForCurrentView()) {
    view.SetPreferredMinSize({ 400, 400 });

    const auto title_bar = view.TitleBar();

    title_bar.ButtonBackgroundColor(Colors::Transparent());
    title_bar.BackgroundColor(Colors::Transparent());
    title_bar.ButtonInactiveBackgroundColor(Colors::Transparent());
  }

  core_window.ResizeStarted([&](CoreWindow const&, impl::IInspectable const&) {

    _state->interactive_resize = true;

    auto &loop = _state->game_loop;

    if (loop) {

      // ...so that we can lock its execution in-order
      loop->check_for_lock = true;

      // wait game loop frame complete...
      std::unique_lock<std::mutex> lock{ _state->mutex };
      loop->target_lock.wait(lock);
    }

    auto &chain = _state->swap_chain;

    if (chain) {
      chain->swaps_immediately = true;
    }
  });

  core_window.ResizeCompleted([&](CoreWindow const& cw, IInspectable const&) {

    auto &chain = _state->swap_chain;

    const auto size = cw.Bounds();

    // resizing the swapchain one last time
    if (chain) {
      chain->resize({ static_cast < int >(size.Width), static_cast < int >(size.Height) });
      chain->swaps_immediately = false;
    }

    auto &loop = _state->game_loop;

    // then run a game loop frame
    if (loop) {
      loop->frame();

      // then give back execution of the frame back to the game loop
      loop->check_for_lock = false;
      loop->loop_lock.notify_all();
    }

    _state->interactive_resize = false;
  });

  core_window.SizeChanged([&](CoreWindow const& cw, WindowSizeChangedEventArgs const& args) {

    // this flag is for when the window is interactively resized, that is, 
      // there is a resize drag going on.
      // the execution order is different since the frame locks are done at
      // the interactive resize begin/end.
    const auto interactive_resize = _state->interactive_resize;

    auto &loop = _state->game_loop;
    auto &chain = _state->swap_chain;

    // if this window is not interactively resizing (maximize, restore operations)
    if (!interactive_resize) {

      if (loop) {

        // ...so that we can lock its execution in-order
        loop->check_for_lock = true;

        // wait game loop frame complete...
        std::unique_lock<std::mutex> lock{ _state->mutex };
        loop->target_lock.wait(lock);
      }
    }

    // this thread now owns the framing...
    if (chain) {
      // ...start by resizing the swapchain
      chain->resize({ cw.Bounds().Width, cw.Bounds().Height });
    }

    // ...then run a game loop frame
    if (loop) {
      loop->frame();

      // ...then give back execution of the frame back to the game loop
      if (!interactive_resize) {
        loop->check_for_lock = false;
        loop->loop_lock.notify_all();
      }
    }
  });
}

window::window(window &&mov) noexcept
  : _state(std::move(mov._state))
  , _handle(mov._handle) {

  mov._handle = 0;
}

window&
  window::operator=(window &&mov) noexcept {

  _state = std::move(mov._state);
  _handle = mov._handle;

  mov._handle = 0;

  return *this;
}



window::~window() {
  close();
}

void
  window::close() {
  
}

cx::game_loop &
  window::game_loop() const {
  
}

void
  window::game_loop(cx::exp::ptr<cx::game_loop> &&value) {
  
}

std::string
  window::title() const {
  
}

void
  window::title(std::string const &value) {
  
}

cx::rect
  window::frame() const {
  
}

void
  window::frame(cx::rect const &value) {
  
}

cx::size_2d
  window::size() const {
  
}

bool
  window::full_screen() const {
  
}

void
  window::full_screen(bool value) {
  
}

bool
  window::interactive_resize() const {
  return _state->interactive_resize;
}

cx::driver_handle
  window::handle() const {
  return _handle;
}

cx::exp::ptr_ref<internal_state>
  window::state() const {
  return cx::exp::ptr_ref{ _state };
}

std::function<void()>
  window::on_size(event<void, window const &, window_event_size const &> const &value) {
  return []() {

  };
}

std::function<void()>
  window::on_close(event<void, window const &> const &value) {
  return []() {

  };
}

std::function<void()>
  window::on_state(event<void, window const &, window_states const &> const &value) {
  return []() {
    
  };
}

bool
  window::operator<(const window &rhs) const {
  return _handle < rhs._handle;
}

bool
  window::operator==(const window &rhs) const {
  return _handle == rhs._handle;
}

SNOW_OWL_NAMESPACE_END
