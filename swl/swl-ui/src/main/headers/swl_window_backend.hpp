//
// Created by Michael Ong on 4/4/20.
//
#pragma once

#include <header.hpp>

#include <map>

#include "window.hpp"
#include "window_surface.hpp"

#include "application.hpp"

SNOW_OWL_NAMESPACE(ui)

struct application;

SNOW_OWL_NAMESPACE_END

SNOW_OWL_NAMESPACE(ui::backend)

struct window_backend {

  static window_backend* instance;


  application* application{};

  std::map< window const*, void* >    native_handles;


  /**
   * @brief Creates a native backing window.
   * @param window A window construct to be created a native window.
  */
  void  create        (window const* window);

  /**
   * @brief Links and configures a platform-created window.
   * @param window A window construct to be linked by a native window.
   * @param native The platform-created window.
  */
  void  create_native (window const* window, void* native);


  void  destroy       (window const* window);

  void  fullscreen    (window const* window);


  void  update_title  (window const* window);

  void  update_frame  (window const* window);


  void* surface       (window const* window);


  cx::exp::ptr_ref<window>
        main_window   ();


  friend struct ui::application;
};

SNOW_OWL_NAMESPACE_END
