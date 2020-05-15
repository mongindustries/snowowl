#undef _USE_MATH_DEFINES
#define NOMINMAX

#include "swl_window_backend.hpp"
#include "swl_win32_window.hpp"

#include <stdexcept>
#include <limits>

#include "swl_window_sink.hpp"

using namespace std;

using namespace swl::cx;

using namespace swl::ui;
using namespace backend;

window_backend* window_backend::instance = new window_backend();

window __main_window = window{nullptr};

void  window_backend::create        (window const* window) {

  win32_window* win32Win = new win32_window(GetModuleHandle(nullptr), window->get_title(), window->get_frame());
  win32Win->reference = window;

  native_handles.emplace(window, win32Win);

  if (window->get_size().components == window::fullscreen_size.components) {
    const_cast<ui::window*>(window)->set_fullscreen(true);
  } else {
    ShowWindow(win32Win->hwnd, SW_SHOW | SW_SHOWNORMAL);
  }

  UpdateWindow  (win32Win->hwnd);
}

void  window_backend::update_title  (window const* window) {
  try {
    const HWND handle = static_cast<win32_window*>(native_handles.at(window))->hwnd;

    wstring newTitle;
    const auto titleSize = MultiByteToWideChar(CP_UTF8, 0, window->get_title().c_str(), -1, nullptr, 0);

    newTitle.resize(titleSize);
    MultiByteToWideChar(CP_UTF8, 0, window->get_title().c_str(), -1, newTitle.data(), titleSize);

    SetWindowText(handle, newTitle.c_str());
  }
  catch (const out_of_range&) {}
}

void  window_backend::update_frame  (window const* window) {
  try {
    const HWND handle = static_cast<win32_window*>(native_handles.at(window))->hwnd;

    auto frame = window->get_frame();
    RECT windowRect{};

    GetClientRect(handle, &windowRect);

    SetWindowPos(handle, nullptr,
      int(frame.origin.x()), int(frame.origin.y()),
      int(windowRect.right - windowRect.left), int(windowRect.bottom - windowRect.top), SWP_FRAMECHANGED);
  }
  catch (const out_of_range&) {}
}

void  window_backend::destroy       (window const* window) {

  auto* const instance = static_cast<win32_window*>(native_handles.at(window));
  DestroyWindow(instance->hwnd);

  delete instance;
  native_handles.erase(window);

  if (native_handles.empty()) {
    PostQuitMessage(0);
  }
}

void  window_backend::fullscreen    (window const* window) {

  const auto result = native_handles.find(window);

  if (result == native_handles.end()) {
    return;
  }

  const auto win32 = static_cast<win32_window*>(result->second);

  if (window->get_fullscreen()) {
    SetWindowLongPtr(win32->hwnd, GWL_STYLE, WS_POPUP | WS_THICKFRAME | WS_SYSMENU | WS_MAXIMIZEBOX);
  } else {
    SetWindowLongPtr(win32->hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
  }

  SetWindowPos(win32->hwnd, HWND_TOP, 0, 0, 0, 0, SWP_ASYNCWINDOWPOS | SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOMOVE);
  ShowWindow(win32->hwnd, SW_SHOWMAXIMIZED);

  RECT newRect{};
  GetClientRect(win32->hwnd, &newRect);
  const_cast<ui::window*>(window)->get_sink()->frame(rect{
                                                       {static_cast<float>(newRect.left), static_cast<float>(newRect.top)},
                                                       {newRect.right - newRect.left, newRect.bottom - newRect.top}
                                                     });
}

void* window_backend::surface       (window const* window) {

  const auto result = native_handles.find(window);

  if (result != native_handles.end()) {
    return static_cast<win32_window*>(result->second)->hwnd;
  }

  return nullptr;
}

window window_backend::main_window() {
 
  if (__main_window.get_handle() == std::numeric_limits<cx::driver_handle>::max()) {
    __main_window = window("Game Window", rect{{100, 100}, {800, 600}});
  }

  return __main_window;
}
