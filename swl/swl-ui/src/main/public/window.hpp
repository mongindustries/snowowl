//
// Created by Michael Ong on 31/3/20.
//
#pragma once

#include <vector>
#include <string>
#include <condition_variable>

#include <header.hpp>
#include <core.hpp>
#include <rect.hpp>
#include <ownership.hpp>
#include <game_loop.hpp>

#include "event.hpp"

SNOW_OWL_NAMESPACE(gx)

struct graphics_swap_chain;

SNOW_OWL_NAMESPACE_END

SNOW_OWL_NAMESPACE(ui)

struct application;


struct window_sink;

struct window_surface;

/**
 * A window construct.
 *
 * This encapsulates a platform-native window. Basic operations such as
 * updating the title and frame are provided.
 *
 * To create a window, use <code>swl::ui::application</code>'s <code>create_window</code>
 * method or instantiate a window using any of the constructors of this object. Alternatively,
 * for platforms that requires a "main" window to be shown (in the case of Android, iOS, and UWP)
 * use <code>swl::ui::application</code>'s <code>get_main_window</code> method.
 *
 * To establish interactivity, bind a <code>swl::cx::game_loop</code> using the method
 * <code>bind_loop</code>. Take note that actions referring to the window inside the game
 * loop must be synchronised as the game loop runs in a separate thread.
 *
 * The window exposes a swap chain variable for display output. A valid
 * <code>swl:gx::graphics_swap_chain</code> instance is linked to this window
 * when created from a <code>swl::gx::graphics_context</code>.
 */
struct SWL_EXPORT window final { SWL_REFERENCEABLE(window)

	/// Possible window states.
  enum state {
    stateActive,
    statePaused,
    stateBackground
  };

  /**
   * @brief Creates a new instance with a starting title and frame.
   * @param window_name The title of the window.
   * @param frame The frame of the window.
   * @return A new swl::ui::window instance.
  */
  window  (std::string window_name, const cx::rect& frame);

  /**
   * @brief Creates a new instance with the default settings.
   * @return A new swl::ui::window instance.
  */
  window  ();

  ~window ();

  // window properties

  /**
   * @brief Binds a swl::cx::game_loop to this window.
   * @param game_loop The swl::cx::game_loop to bind.
   * @remarks The game loop runs in a separate thread. TODO: add a dispatcher for window.
   */
  void  bind_loop       (const cx::exp::ptr_ref<cx::game_loop>& game_loop);

  /**
   * Updates the title of the window.
   * @param value The new value to apply.
   */
  void  set_title       (const std::string& value);

  /**
   * Updates the frame of the window.
   * @param value The new value to apply.
   */
  void  set_frame       (const cx::rect& value);

  /**
   * @brief Updates the window if it needs to display full screen.
   * @param value The new value to apply.
   */
  void	set_fullscreen  (bool value);

  /**
   * Returns the window's title.
   * @return The current value.
   */
  [[nodiscard]] std::string
    get_title           () const;

  /**
   * Returns the window client size.
   * @return The current value.
   */
  [[nodiscard]] cx::size_2d
    get_size            () const;

  /**
   * Returns the window's frame relative to the display.
   * @return The current value.
   */
  [[nodiscard]] cx::rect
    get_frame           () const;

  /**
   * Returns reference to a <code>swl::ui::window_sink</code> instance.
   * This is used exclusively for OS to window interaction. Client-consuming
   * code is expected to not use any methods from the instance this returns.
   * @return A sink for the OS to drop events.
   */
  [[nodiscard]] cx::exp::ptr_ref<window_sink>
    get_sink            () const;

  /**
   * Returns if the window is currently in an active resize mode.
   * Active resize is when the use click-drags a window to change its size.
   *
   * @remarks On macOS, this value returns true on click-drag and zoom size operations.
   *
   * @return
   */
  [[nodiscard]] bool
    is_sizing           () const;

  // window events

  bool  operator<     (const window& rhs) const;

  bool  operator==    (const window& rhs) const;

private:

  typedef std::vector<event<void, const window&>>                   EventCloseList;

  typedef std::vector<event<void, const window&, const state&>>     EventStateList;

  typedef std::vector<event<void, const window&, const cx::rect&>>  EventSizeList;


  cx::driver_handle         handle{ 0 };

  std::string               title{};

  cx::rect                  frame{};

  bool                      resizing{ false };


  cx::exp::ptr<window_sink> sink;

  friend struct window_sink;
  friend struct window_surface;

public:

  /// The swap chain reference of the window.
  cx::exp::ptr_ref<gx::graphics_swap_chain> swap_chain;

  /// The game loop reference of the window.
  cx::exp::ptr_ref<cx::game_loop>           game_loop;

  /// A list of events the window will run upon closing.
  EventCloseList          event_on_close;
  /// A list of events the window will run upon state change.
  EventStateList          event_on_state;
  /// A list of events the window will run upon resize. Take note this will not run when
  /// <code>set_frame</code> is called.
  EventSizeList           event_on_resize;
};

SNOW_OWL_NAMESPACE_END
