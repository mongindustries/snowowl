//
// Created by mong on 5/18/2020.
//
#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <utility>
#include <vector>
#include <unordered_map>

#include <Windows.h>

#include <header.hpp>
#include <game_loop.hpp>

#include "event.hpp"

SNOW_OWL_NAMESPACE(gx)

struct swap_chain;

SNOW_OWL_NAMESPACE_END

SNOW_OWL_NAMESPACE(ui)

struct window;

struct internal_state {
  HWND handle{nullptr};
  bool interactive_resize{false};

  cx::exp::ptr<cx::game_loop> game_loop{nullptr};
  cx::exp::ptr_ref<gx::swap_chain> swap_chain{nullptr};

  std::unordered_map<cx::driver_handle, event<void, window const&>> close_events;
  std::unordered_map<cx::driver_handle, event<void, window const&, window_event_size const&>> size_events;
  std::unordered_map<cx::driver_handle, event<void, window const&, window_states const&>> state_events;
};

struct internal_state_manager {
  static internal_state_manager *instance;
  std::vector<std::pair<internal_state*, window*>> instances;
};

SNOW_OWL_NAMESPACE_END
