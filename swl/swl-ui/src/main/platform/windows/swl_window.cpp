//
// Created by mong on 5/18/2020.
//
#define WIN32_LEAN_AND_MEAN

#include <algorithm>

#include "window.hpp"
#include "swl_internal_state.h"

#include <Windows.h>

SNOW_OWL_NAMESPACE(ui)

internal_state_manager* internal_state_manager::instance = new internal_state_manager{};

struct window_invalid_game_loop: std::exception{};

window::window(nullptr_t): _state(nullptr), _handle(0) {
}

window::window(const std::string &window_name, const cx::rect &frame, void*): _state(new internal_state()) {

  const wchar_t class_name[9] = L"snowowl:";

  size_t length = MultiByteToWideChar(CP_UTF8, 0, window_name.data(), -1, nullptr, 0);

  std::wstring title;
  title.reserve(length);
  MultiByteToWideChar(CP_UTF8, 0, window_name.data(), -1, title.data(), length);

  RECT target_rect{
    static_cast < LONG >(frame.origin.x()),
    static_cast < LONG >(frame.origin.y()),
    static_cast < LONG >(frame.origin.x() + frame.size.x()),
    static_cast < LONG >(frame.origin.y() + frame.size.y()) };

  auto dpi = GetDpiForSystem();
  auto scale = dpi / 96.f;

  target_rect.left = UINT(scale * target_rect.left);
  target_rect.top = UINT(scale * target_rect.top);
  target_rect.right = UINT(scale * target_rect.right);
  target_rect.bottom = UINT(scale * target_rect.bottom);

  AdjustWindowRectExForDpi(&target_rect, WS_OVERLAPPEDWINDOW, false, 0, dpi);

  _state->handle = CreateWindowEx(
    WS_EX_NOREDIRECTIONBITMAP,
    class_name,
    title.c_str(),
    WS_OVERLAPPEDWINDOW,
    target_rect.left,
    target_rect.top,
    target_rect.right - target_rect.left,
    target_rect.bottom - target_rect.top,
    nullptr,
    nullptr,
    GetModuleHandle(nullptr),
    nullptr);

  ShowWindow(_state->handle, SW_SHOW);
  UpdateWindow(_state->handle);

  internal_state_manager::instance->instances.emplace_back(_state.pointer(), this);
}

window::~window() {
  close();
}


window::window(window &&mov) noexcept: _state(std::move(mov._state)), _handle(mov._handle) {

  auto& instances = internal_state_manager::instance->instances;
  const auto iterator = std::find_if(instances.begin(), instances.end(), [&](std::pair<internal_state*, window*> const& item) {
    return item.second->handle() == this->handle();
    });

  if (iterator == instances.end()) {

    mov._handle = 0;
    instances.emplace_back(_state.pointer(), this);

    return;
  }

  instances.erase(iterator);

  instances.emplace_back(_state.pointer(), this);
}

window &
  window::operator=(window &&mov) noexcept {

  _state = std::move(mov._state);
  _handle = mov._handle;

  auto& instances = internal_state_manager::instance->instances;
  const auto iterator = std::find_if(instances.begin(), instances.end(), [&](std::pair<internal_state*, window*> const& item) {
    return item.second->handle() == this->handle();
  });

  if (iterator == instances.end()) {

    mov._handle = 0;
    instances.emplace_back(_state.pointer(), this);

    return *this;
  }

  mov._handle = 0;

  instances.erase(iterator);

  instances.emplace_back(_state.pointer(), this);

  return *this;
}

void window::close() {

  if (_handle == 0) {
    return;
  }

  for (auto &item : _state->close_events) {
    item.second.invoke(*this);
  }

  DestroyWindow(_state->handle);
  
  auto &instances = internal_state_manager::instance->instances;
  const auto iterator = std::find_if(instances.begin(), instances.end(), [&](std::pair<internal_state *, window *> const&item) {
    return item.second->handle() == this->handle();
  });

  if (iterator == instances.end()) {
    return;
  }

  instances.erase(iterator);
  _state->handle = nullptr;
  _state.release();

  _handle = 0;

  if (instances.empty()) {
    PostQuitMessage(0);
  }
}

cx::game_loop& window::game_loop() const {
  if (!_state->game_loop) {
    throw window_invalid_game_loop();
  }

  return _state->game_loop.get();
}

void window::game_loop(cx::exp::ptr<cx::game_loop> &&value) {
  _state->game_loop = std::move(value);
}


std::string window::title() const {

  const size_t size = GetWindowTextLength(_state->handle);

  std::wstring text;
  text.reserve(size);

  GetWindowText(_state->handle, text.data(), size);

  const size_t mb_size = WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1, nullptr, 0, nullptr, nullptr);

  std::string result;
  result.reserve(mb_size);

  WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1, result.data(), result.size(), nullptr, nullptr);

  return result;
}

void window::title(const std::string &value) {

  const size_t wide_size = MultiByteToWideChar(CP_UTF8, 0, value.c_str(), -1, nullptr, 0);

  std::wstring title;
  title.reserve(wide_size);

  MultiByteToWideChar(CP_UTF8, 0, value.c_str(), -1, title.data(), wide_size);

  SetWindowText(_state->handle, title.c_str());
}


cx::rect window::frame() const {

  RECT w_rect{};
  GetWindowRect(_state->handle, &w_rect);

  RECT c_rect{};
  GetClientRect(_state->handle, &c_rect);

  const auto scale = static_cast <float>(GetDpiForSystem()) / 96.f;

  const auto offset_x = w_rect.right   - w_rect.left - c_rect.right;
  const auto offset_y = w_rect.bottom  - w_rect.top  - c_rect.bottom;

  const auto rect = cx::rect{
      {
        (float) (w_rect.left + offset_x),
        (float) (w_rect.top + offset_y) },
      {
        c_rect.right,
        c_rect.bottom } };

  return scaled(rect, 1 / scale);
}

void window::frame(const cx::rect &value) {

  RECT window_rect{ 0, 0, value.size.x(), value.size.y() };
  AdjustWindowRectExForDpi(&window_rect, WS_OVERLAPPEDWINDOW, false, 0, GetDpiForWindow(_state->handle));

  SetWindowPos(
    _state->handle,
    HWND_TOP,
    static_cast < int >(value.origin.x()),
    static_cast < int >(value.origin.y()),
    static_cast < int >((window_rect.right - window_rect.left)),
    static_cast < int >((window_rect.bottom - window_rect.top)),
    SWP_SHOWWINDOW | SWP_ASYNCWINDOWPOS);
}

cx::size_2d window::size() const {
  return frame().size;
}


bool window::full_screen() const {
  return GetWindowLong(_state->handle, GWL_STYLE); // TODO
}

void window::full_screen(bool value) {
  /*
  if (value) {
    SetWindowLong(_state->handle, GWL_STYLE, WS_POPUP | WS_VISIBLE | WS_THICKFRAME | WS_MAXIMIZEBOX);
  } else {
    SetWindowLong(_state->handle, GWL_STYLE, WS_OVERLAPPEDWINDOW);
  }
  */

  // SetWindowPos(_state->handle, HWND_TOP, 0, 0, 0, 0, SWP_ASYNCWINDOWPOS | SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);

  // if (value) {
    // ShowWindow(_state->handle, SW_SHOWMAXIMIZED);
  // }
}


bool window::interactive_resize() const {
  return _state->interactive_resize;
}


cx::driver_handle window::handle() const {
  return _handle;
}

cx::exp::ptr_ref<internal_state>
  window::state() const {
  return cx::exp::ptr_ref{ _state };
}

std::function<void()>
  window::on_size(event<void, window const &, window_event_size const &> const &value) {

  cx::driver_handle handle = cx::core::make_handle();
  _state->size_events.emplace(handle, value);

  return [&, handle]() {
    const auto result = _state->size_events.find(handle);

    if (result == _state->size_events.end()) {
      _state->size_events.erase(result);
    }
  };
}

std::function<void()>
  window::on_close(event < void, window const & > const &value) {

  cx::driver_handle handle = cx::core::make_handle();
  _state->close_events.emplace(handle, value);

  return [&, handle]() {
    const auto result = _state->close_events.find(handle);

    if (result == _state->close_events.end()) {
      _state->close_events.erase(result);
    }
  };
}

std::function<void()>
  window::on_state(event < void, window const &, window_states const & > const &value) {

  cx::driver_handle handle = cx::core::make_handle();
  _state->state_events.emplace(handle, value);

  return [&, handle]() {
    const auto result = _state->state_events.find(handle);

    if (result == _state->state_events.end()) {
      _state->state_events.erase(result);
    }
  };
}


bool window::operator<(const window &rhs) const {
  return _handle < rhs._handle;
}

bool window::operator==(const window &rhs) const {
  return _handle == rhs._handle;
}

SNOW_OWL_NAMESPACE_END
