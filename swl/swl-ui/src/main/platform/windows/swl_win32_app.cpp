//
// Created by micha on 4/3/2020.
//
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>

#include "application.hpp"

#include "window.hpp"

#include "swl_window_sink.hpp"
#include "swl_window_backend.hpp"
#include "swl_win32_window.hpp"

#include <Windows.h>
#include <dwmapi.h>

#include <directx/swap_chain.h>
#include <directx/queue.h>

using namespace std;
using namespace swl;

std::mutex proc_mutex;


ui::backend::win32_window*
      win32WindowFromHWND (HWND hwnd) {

  for (const auto& item : ui::backend::window_backend::instance->native_handles) {

    auto winItem = static_cast<ui::backend::win32_window*>(get<1>(item));

    if (winItem->hwnd == hwnd) {
      return winItem;
    }
  }

  return nullptr;
}

ui::window*
      windowFromHWND      (HWND hwnd) {
  auto result = win32WindowFromHWND(hwnd);

  if (!result) {
    return nullptr;
  }

  return const_cast<ui::window*>(result->reference);
}

void  updateSizeLock      (HWND hwnd, ui::window* window) {

  RECT rect{};
  GetClientRect(hwnd, &rect);

  window->get_sink()->frame(cx::rect{
    { static_cast<float>(rect.left), static_cast<float>(rect.top) },
    { rect.right - rect.left, rect.bottom - rect.top }
    });
}

LRESULT CALLBACK
      win32_windowProc    (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {

  switch (message) {
  case WM_ENTERSIZEMOVE: {
    // TODO: check if we're only moving the window, not resizing it.

    if (const auto window = windowFromHWND(hwnd)) {
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
    }
    return 0;
  }
  case WM_EXITSIZEMOVE: {
    // TODO: check if we're only moving the window, not resizing it.
    if (const auto window = windowFromHWND(hwnd)) {
      updateSizeLock(hwnd, window);

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
    }

    return 0;
  }
  case WM_WINDOWPOSCHANGED: {

    if (auto window = windowFromHWND(hwnd)) {

      WINDOWPOS* data = reinterpret_cast<WINDOWPOS*>(lparam);

      if (data->flags & SWP_NOSIZE) {
        return 0;
      }

      // this flag is for when the window is interactively resized, that is, 
      // there is a resize drag going on.
      // the execution order is different since the frame locks are done at
      // the interactive resize begin/end.
      bool interactive_resize = window->is_sizing();

      updateSizeLock(hwnd, window);

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
    }

    return 0;
  }
  case WM_CLOSE: {
    if (auto* const window = windowFromHWND(hwnd)) {

      if (window->game_loop) {

        // wait game loop frame complete...
        std::unique_lock<std::mutex> lock{ proc_mutex };
        window->game_loop->target_lock.wait(lock);
        window->game_loop->close();
      }

      window->get_sink()->closed();
    }
    return 0;
  }
  case WM_GETMINMAXINFO: {
    reinterpret_cast<MINMAXINFO*>(lparam)->ptMinTrackSize = { 400, 400 };
    return 0;
  }
  case WM_QUIT:
    PostQuitMessage(0);
    return 0;
  default:
    return DefWindowProc(hwnd, message, wparam, lparam);
  }
}


void  ui::application::pre_heat         (application& app) {

  HINSTANCE instance = (HINSTANCE)app.native_instance;

  constexpr wchar_t* className = L"snowowl: window";
  WNDCLASSEX customClass{ };

  customClass.style = CS_HREDRAW | CS_VREDRAW;
  customClass.cbWndExtra = 0;
  customClass.cbClsExtra = 0;

  customClass.cbSize = sizeof(WNDCLASSEX);
  customClass.lpfnWndProc = win32_windowProc;
  customClass.hInstance = instance;
  customClass.lpszClassName = className;
  customClass.hIcon = LoadIcon(instance, MAKEINTRESOURCE(108));
  customClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
  customClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
  customClass.hIconSm = LoadIcon(instance, MAKEINTRESOURCE(109));

  RegisterClassEx(&customClass);
}

int   ui::application::run_loop         (application& app) {

  MSG msg{ };
  int res = 0;

  app.on_create();

  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    res = DispatchMessage(&msg);
  }

  app.on_destroy();

  return res;
}

cx::exp::ptr_ref<ui::window>
      ui::application::get_main_window  () {
  return backend::window_backend::instance->main_window();
}
