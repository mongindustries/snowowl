#pragma once

#include <mutex>

#include <header.hpp>
#include <game_loop.hpp>

#include <swap_chain.hpp>

#include <winrt/base.h>

#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.ApplicationModel.Core.h>

using namespace winrt;

SNOW_OWL_NAMESPACE(ui)

struct internal_state {

  agile_ref<Windows::UI::Core::CoreWindow> core_window;
  bool interactive_resize;

  cx::exp::ptr<cx::game_loop> game_loop;
  cx::exp::ptr_ref<gx::swap_chain> swap_chain;

  std::mutex mutex;
};

SNOW_OWL_NAMESPACE_END
