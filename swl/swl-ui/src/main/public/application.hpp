//
// Created by micha on 4/3/2020.
//
#pragma once

#include <type_traits>
#include <functional>

#include <header.hpp>

#include "window.hpp"

SNOW_OWL_NAMESPACE(ui::backend)

struct window_backend;

SNOW_OWL_NAMESPACE_END

SNOW_OWL_NAMESPACE(ui)

/**
 * An application construct.
 *
 * This encapsulates application startup behavior. Client consuming
 * code is expected to inherit from this object and override <code>on_create</code>
 * and <code>on_destroy</code> methods to setup application logic.
 *
 * A main window is obtained by calling <code>get_main_window</code>. On certain
 * platforms like Win32 and macOS, this method returns a <code>swl::ui::window</code>
 * lazily. But on platforms like Android, iOS, and UWP this method returns a valid
 * value by the time <code>on_create</code> is called.
 *
 * To run an application instance, use the static method <code>run</code> that has
 * a single parameter <code>app</code>:
 *
 * @code
 *
 * int main(char* const* args, int arg_count) {
 *   game::application_subclass app;
 *
 *   return application::run(app);
 * }
 *
 * @endcode
 *
 */
struct SWL_EXPORT application { SWL_REFERENCEABLE(application) SWL_POLYMORPHIC(application)

  /**
   * Creates a new instance with the specified native application interface.
   * @param native_instance The native instance provided by the platform for application initialization.
   */
  explicit application  (void* native_instance);

  // methods

  /**
   * Obtains the main window object.
   * @return A reference to the main window.
   */
  static std::reference_wrapper<window>
    main_window ();

  // hooks

  /// Method called when the application is created but its run loop has not started.
  virtual void
    on_create   () {
  }

  /// Method called before the application is destroyed.
  virtual void
    on_destroy  () {
  }

  /**
   * Method to run the application.
   * @tparam App The application type.
   * @param app The application instance to run.
   * @return The application status code.
   */
  template<typename App>
  static std::enable_if_t< std::is_base_of_v<application, App>, int > run(App app) {
            App::pre_heat(app);
    return  App::run_loop(app);
  }

  friend struct backend::window_backend;

private:

  void* native_instance = nullptr;

  static void
    pre_heat(application& app);

  static int
    run_loop(application& app);
};

SNOW_OWL_NAMESPACE_END
