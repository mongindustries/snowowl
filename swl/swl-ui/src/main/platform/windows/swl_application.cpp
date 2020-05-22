//
// Created by micha on 4/3/2020.
//
#include <iostream>
#include <sstream>
#include <cassert>

#include "application.hpp"
#include "window.hpp"

#include "swl_internal_state.h"

#include <Windows.h>
#include <dwmapi.h>

#include <directx/swap_chain.h>
#include <directx/queue.h>

using namespace std;
using namespace swl;

std::mutex proc_mutex;

struct application_bad_hwnd : std::exception { };

std::reference_wrapper < std::pair < ui::internal_state *, ui::window * > >
  instance_from_hwnd(HWND hwnd) {

  auto &instances = ui::internal_state_manager::instance->instances;
  auto  iterator  = std::find_if(instances.begin(), instances.end(), [hwnd](pair < ui::internal_state *, ui::window * > const &item) { return item.first->handle == hwnd; });

  if (iterator == instances.end()) { throw application_bad_hwnd(); }

  return *iterator;
}

std::map < UINT, std::function < LRESULT(std::pair < ui::internal_state *, ui::window * > &, WPARAM, LPARAM) > >
  message_defs = {
    {
        WM_ENTERSIZEMOVE, [](std::pair < ui::internal_state *, ui::window * > &instance, WPARAM wparam, LPARAM lparam) -> LRESULT {
          instance.first->interactive_resize = true;

          auto loop = cx::exp::ptr_ref < cx::game_loop >{instance.first->game_loop};
          const auto swap = instance.first->swap_chain;

          if (loop) {
            loop->check_for_lock = true;

            std::unique_lock < std::mutex > lock{proc_mutex};
            loop->target_lock.wait(lock);
          }

          if (swap) { swap->swaps_immediately = true; }

          return 0;
        }
    },
    {
        WM_EXITSIZEMOVE, [](std::pair < ui::internal_state *, ui::window * > &instance, WPARAM wparam, LPARAM lparam) -> LRESULT {
          
          auto loop = cx::exp::ptr_ref < cx::game_loop >{ instance.first->game_loop };
          auto swap = instance.first->swap_chain;

          if (swap) {
            RECT client_size{};
            GetClientRect(instance.first->handle, &client_size);

            swap->resize({client_size.right, client_size.bottom});
            swap->swaps_immediately = false;
          }

          if (loop) {
            loop->frame();

            loop->check_for_lock = false;
            loop->loop_lock.notify_all();
          }

          instance.first->interactive_resize = false;

          return 0;
        }
    },
    {
        WM_WINDOWPOSCHANGED, [](std::pair < ui::internal_state *, ui::window * > &instance, WPARAM wparam, LPARAM lparam) -> LRESULT {
          auto *const window_pos = reinterpret_cast<WINDOWPOS*>(lparam);
          ui::internal_state *state = instance.first;

          if (window_pos->flags & SWP_NOSIZE) {
            return 0;
          }

          const bool interactive_resize = state->interactive_resize;

          if (!interactive_resize) {
            (void) message_defs[WM_ENTERSIZEMOVE](instance, wparam, lparam);
          }

          if ( interactive_resize) {
            auto swap = state->swap_chain;

            for (auto &item : state->size_events) {
              item.second.invoke(*instance.second,
                ui::window_event_size{
                  state->interactive_resize,
                  { window_pos->cx, window_pos->cy } });
            }

            if (swap) {
              swap->resize({window_pos->cx, window_pos->cy});
            }

            auto loop = cx::exp::ptr_ref{state->game_loop};

            if (loop) {
              loop->frame();
            }

          } else {
            (void) message_defs[WM_EXITSIZEMOVE](instance, wparam, lparam);
          }

          return 0;
        }
    },
    {
        WM_CLOSE, [](std::pair < ui::internal_state *, ui::window * > &instance, WPARAM wparam, LPARAM lparam) -> LRESULT {
          instance.second->close(); // close window!
          return 0;
        }
    },
    {
        WM_GETMINMAXINFO, [](std::pair < ui::internal_state *, ui::window * > &instance, WPARAM wparam, LPARAM lparam) -> LRESULT {

          const auto scale = static_cast < float >(GetDpiForWindow(get < 0 >(instance)->handle))
                             / 96;

          reinterpret_cast < MINMAXINFO * >(lparam)->ptMinTrackSize = {
              (LONG) std::floor(400 * scale), (LONG) std::floor(400 * scale)
          };
          return 0;
        }
    }
};

ui::window __main_window{nullptr};

LRESULT
  win32_windowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
  try {
    const auto instance = instance_from_hwnd(hwnd);
    const auto iterator = message_defs.find(message);

    if (iterator == message_defs.end()) { return DefWindowProc(hwnd, message, wparam, lparam); }

    return iterator->second(instance.get(), wparam, lparam);
  } catch (application_bad_hwnd const &) { return DefWindowProc(hwnd, message, wparam, lparam); }
}


void
  ui::application::pre_heat(application &app) {

  const auto instance = static_cast < HINSTANCE >(app.native_instance);

  constexpr wchar_t class_name[9] = L"snowowl:";
  WNDCLASSEX        custom_class{};

  custom_class.style         = CS_HREDRAW | CS_VREDRAW;
  custom_class.cbWndExtra    = 0;
  custom_class.cbClsExtra    = 0;
  custom_class.cbSize        = sizeof(WNDCLASSEX);
  custom_class.lpfnWndProc   = win32_windowProc;
  custom_class.hInstance     = instance;
  custom_class.lpszClassName = class_name;
  custom_class.hIcon         = LoadIcon(instance, MAKEINTRESOURCE(108));
  custom_class.hCursor       = LoadCursor(nullptr, IDC_ARROW);
  custom_class.hbrBackground = static_cast < HBRUSH >(GetStockObject(BLACK_BRUSH));
  custom_class.hIconSm       = LoadIcon(instance, MAKEINTRESOURCE(109));

  RegisterClassEx(&custom_class);

  SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
}

int
  ui::application::run_loop(application &app) {

  MSG msg{};
  int res = 0;

  app.on_create();

  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    res = DispatchMessage(&msg);
  }

  app.on_destroy();

  return res;
}

std::reference_wrapper<ui::window>
  ui::application::main_window() {

  if (!__main_window.handle()) {
    __main_window = window{ "Main Window", { {100, 100}, {800, 480} }};
  }

  return __main_window;
}
