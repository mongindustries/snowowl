#include "swl_window_backend.hpp"
#include "swl_win32_window.hpp"

#include <stdexcept>

using namespace std;

using namespace swl::cx;

using namespace swl::ui;
using namespace backend;

window_backend* window_backend::instance = new window_backend();

void  window_backend::create        (window const* window) {

  auto  win32Win            = new win32_window(GetModuleHandle(nullptr), window->get_title(), window->get_frame());
        win32Win->reference = window;

  ShowWindow    (win32Win->hwnd, SW_SHOW | SW_SHOWNORMAL);
  UpdateWindow  (win32Win->hwnd);

  native_handles.emplace(window, win32Win);
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

  auto win32 = static_cast<win32_window*>(result->second);

  if (window->get_fullscreen()) {
    SetWindowLongPtr(win32->hwnd, GWL_STYLE, WS_POPUP | WS_SYSMENU | WS_MAXIMIZE);
  } else {
    SetWindowLongPtr(win32->hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
  }

  // todo: update size
}

void* window_backend::surface       (window const* window) {

  const auto result = native_handles.find(window);

  if (result != native_handles.end()) {
    return static_cast<win32_window*>(result->second)->hwnd;
  }

  return nullptr;
}
