//
// Created by Michael Ong on 31/3/20.
//
#pragma once

#include <string>
#include <functional>

#include <header.hpp>
#include <core.hpp>
#include <rect.hpp>
#include <game_loop.hpp>
#include <ownership.hpp>

#include "event.hpp"

SNOW_OWL_NAMESPACE(gx)

struct swap_chain;

SNOW_OWL_NAMESPACE_END

SNOW_OWL_NAMESPACE(ui)

struct application;

struct window_surface;

struct internal_state;

struct window_event_size {
  bool interactive;
  cx::size_2d new_size;
};

enum window_states {
  stateInactive,
  stateBackground,
  stateActive
};

/**
 * A window construct.
 *
 * This encapsulates a platform-native window. Basic operations such as
 * updating the title and frame are provided.
 *
 * To create a window, instantiate a window using any of the constructors of this object. Alternatively,
 * for platforms that requires a "main" window to be shown (in the case of Android, iOS, and UWP)
 * use the <code>application::get_main_window</code> method.
 *
 * To establish interactivity, bind a <code>swl::cx::game_loop</code> using the method
 * <code>bind_loop</code>. Take note that actions referring to the window inside the game
 * loop must be synchronized as the game loop runs in a separate thread.
 *
 * The window exposes a swap chain variable for display output. A valid
 * <code>swap_chain</code> instance is linked to this window
 * when created from a <code>factory</code>.
 */
struct SWL_EXPORT window final { SWL_REFERENCEABLE(window)

  static cx::size_2d fullscreen_size;

  /// Possible window states.
  enum state {
    stateActive,
    statePaused,
    stateBackground
  };


  /**
   * @brief Creates a null window instance.
   * @return A new null instance.
  */
  explicit window(nullptr_t);

  /**
   * @brief Creates a new instance with a starting title and frame.
   * @param window_name The title of the window.
   * @param frame The frame of the window.
   * @param instance
   * @return A new swl::ui::window instance.
  */
  window(const std::string &window_name, cx::rect const &frame, void* instance = nullptr);

  ~window();


  void
    close();


  [[nodiscard]] cx::game_loop&
    game_loop() const;

  void
    game_loop(cx::exp::ptr<cx::game_loop> &&value);


  [[nodiscard]] std::string
    title() const;
  void
    title(std::string const &value);

  [[nodiscard]] cx::rect
    frame() const;
  void
    frame(cx::rect const &value);

  [[nodiscard]] cx::size_2d
    size() const;


  [[nodiscard]] bool
    full_screen() const;
  void
    full_screen(bool value);


  [[nodiscard]] bool
    interactive_resize() const;

  [[nodiscard]] cx::driver_handle
    handle() const;

  [[nodiscard]] cx::exp::ptr_ref<internal_state>
    state() const;


  std::function<void()>
    on_size(event<void, window const&, window_event_size const&> const& value);

  std::function<void()>
    on_close(event<void, window const&> const& value);

  std::function<void()>
    on_state(event<void, window const&, window_states const&> const& value);


  bool
    operator<(const window &rhs) const;

  bool
    operator==(const window &rhs) const;

private:

  cx::exp::ptr<internal_state> _state;

  cx::driver_handle _handle;

  friend struct window_surface;
};

SNOW_OWL_NAMESPACE_END
