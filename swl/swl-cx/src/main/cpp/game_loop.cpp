//
// Created by Michael Ong on 12/4/20.
//

#include <thread>
#include <chrono>
#include <iostream>

#include "game_loop.hpp"

#ifdef SWL_UWP
#include <winrt/base.h>
#endif

using namespace std;
using namespace chrono;
using namespace swl::cx;

game_loop::game_loop() noexcept: target_frame_time(1000ms / 60), target_update_count(4) { }

game_loop::game_loop(uint16_t targetFramerate, uint16_t bailAmount): target_frame_time(1000ms / targetFramerate), target_update_count(bailAmount) { }

game_loop::~game_loop() = default;


void game_loop::open() {

  auto func = [](game_loop* game_loop) {

#ifdef SWL_UWP
    winrt::init_apartment();
#endif // SWL_UWP

    game_loop->game_thread_id = this_thread::get_id();

    game_loop->create();

    game_loop->t1 = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());

    auto      accu    = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
    auto      tt      = accu;

    uint16_t  frames  = 0;


    while (game_loop->running) {

      // when check_for_lock is enabled, the game loops checks this every frame.
      // the use for this check is when other threads other than the game loop wants to lock loop
      // execution (because of OS-level stuff, eg: window resize)
      if (game_loop->check_for_lock) {

        std::unique_lock<std::mutex> lock{ game_loop->loop_mutex };
        game_loop->loop_lock.wait(lock);
      }

      game_loop->frame();

      // notify any threads waiting for this game loop iteration's execution to complete.
      game_loop->target_lock.notify_all();

      accu       = duration_cast<seconds>(high_resolution_clock::now().time_since_epoch());
      frames    += 1;

      if ((accu - tt) >= 1s) {
        if (game_loop->frame_callback) {
          game_loop->frame_callback(frames);
        }

        frames  = 0;
        tt      = accu;
      }
    }
  };

  running = true;

  game_thread = thread(func, this);
  game_thread.detach();
}

void game_loop::close() {

  running = false;
}

void game_loop::frame() {

  const auto current = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
  const auto offset = current - t1;

  accumulate += offset;

  uint16_t frameCount = 0;
  while (accumulate >= target_frame_time && frameCount < target_update_count) {
    update(target_frame_time);

    accumulate -= target_frame_time;
    frameCount += 1;

    if (frameCount == target_update_count) {
      accumulate = 0ms;
    }
  }

  t1 = current;

  render(accumulate / target_frame_time);
}

bool game_loop::is_in_game_thread() const
{
  return this_thread::get_id() == game_thread_id;
}
