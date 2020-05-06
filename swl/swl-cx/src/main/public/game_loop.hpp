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

/**
 * A game loop construct.
 *
 * A game loop manages game/app execution by executing two functions,
 * namely update and render on an infinite loop.
 *
 * A game loop has an update tick that is time-consistent given a
 * <code>target_frame_rate</code> and a <code>bail_amount</code>. Bail
 * amount is used in the off chance that a frame has been delayed far
 * too long (due to the game loop's execution blocked by a long render
 * operation or the game loop is waiting for the next frame to be notified
 * by another thread.
 *
 * A game loop executes the two methods in a separate thread. This ensures
 * that the event thread by the OS is not interfered in any way and event
 * pumping is consistent. This helps with input latency by separately processing
 * input stuff on another thread.
 *
 * To define a event loop like app loop, define an arbitrary frame rate and set
 * bail amount to 1.
 *
 * A game loop's frame can also be manually executed by calling the <code>frame</code>
 * method. But be sure to block the game thread first by setting <code>check_for_lock</code>
 * to true.
 *
 * To start a game loop, call the <code>open</code> method. To close it, call the <code>close</code>
 * method. Be sure to wait for game loop frame execution completion by doing a <code>wait</code>
 * call on <code>target</code> lock.
 *
*/
struct game_loop { SWL_NO_CPY_CTOR(game_loop) SWL_NO_MOVE_CTOR(game_loop) SWL_POLYMORPHIC(game_loop)

  game_loop(uint16_t targetFramerate, uint16_t bailAmount);


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
