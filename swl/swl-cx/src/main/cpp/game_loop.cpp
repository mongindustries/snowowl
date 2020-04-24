//
// Created by Michael Ong on 12/4/20.
//

#include <thread>
#include <chrono>
#include <iostream>

#include "game_loop.hpp"

using namespace std;
using namespace chrono;
using namespace swl::cx;

game_loop::game_loop(uint16_t targetFramerate, uint16_t bailAmount)
: target_frame_time(1000 / targetFramerate),
  target_update_count(bailAmount) {
}

void game_loop::open() {

	auto func = [](game_loop* game_loop) {

		game_loop->create();

		game_loop->t1 = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());

		while (game_loop->running) {
			game_loop->preFrame();
			game_loop->frame();
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
