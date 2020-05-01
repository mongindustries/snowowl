//
// Created by Michael Ong on 12/4/20.
//
#pragma once

#include <mutex>
#include <condition_variable>

#include <thread>
#include <chrono>

#include <functional>

#include "header.hpp"

SNOW_OWL_NAMESPACE(cx)

struct game_loop {

  game_loop(uint16_t targetFramerate, uint16_t bailAmount);

  virtual ~game_loop() = default;


  void  open();

  void  close();

  void  frame();


  virtual void
    create() = 0;

  virtual void
    update(std::chrono::milliseconds delta) = 0;

  virtual void
    render(float offset) = 0;


  bool  is_in_game_thread() const;

  bool                      check_for_lock{ false };
  std::condition_variable   loop_lock;

  std::condition_variable   target_lock;

  std::function<void(uint16_t)>
    frame_callback;

private:

  std::thread::id           game_thread_id;
  std::thread               game_thread;

  std::mutex                loop_mutex;


  std::chrono::milliseconds t1{};
  std::chrono::milliseconds accumulate{};

  bool                      running{ false };

  std::chrono::milliseconds target_frame_time{};
  uint16_t                  target_update_count{};
};

SNOW_OWL_NAMESPACE_END
