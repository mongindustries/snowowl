#pragma once

#include <mutex>

#include <header.hpp>

#include <winrt/base.h>

#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.ApplicationModel.Core.h>

using namespace winrt;

SNOW_OWL_NAMESPACE(cx)

struct game_loop;

SNOW_OWL_NAMESPACE_END

SNOW_OWL_NAMESPACE(gx)

struct swap_chain;

SNOW_OWL_NAMESPACE_END

SNOW_OWL_NAMESPACE(ui)

struct internal_state {

  agile_ref<Windows::UI::Core::CoreWindow> core_window;
  agile_ref<Windows::UI::ViewManagement::ApplicationView> application_view;

  bool interactive_resize;

  cx::exp::ptr<cx::game_loop> game_loop{nullptr};
  cx::exp::ptr_ref<gx::swap_chain> swap_chain;

  std::mutex mutex;
};

SNOW_OWL_NAMESPACE_END
