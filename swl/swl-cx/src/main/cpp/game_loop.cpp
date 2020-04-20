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

GameLoop::GameLoop(uint16_t targetFramerate, uint16_t bailAmount)
: targetFrametime(1000 / targetFramerate),
  maxUpdateCount(bailAmount) {
}

void GameLoop::open() {

	auto func = [](GameLoop* game_loop) {

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

void GameLoop::close() {

	running = false;
}

void GameLoop::frame() {

	const auto current = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
	const auto offset = current - t1;

	accumulate += offset;

	uint16_t frameCount = 0;
	while (accumulate >= targetFrametime && frameCount < maxUpdateCount) {
		update(targetFrametime);

		accumulate -= targetFrametime;
		frameCount += 1;

		if (frameCount == maxUpdateCount) {
			accumulate = 0ms;
		}
	}

	t1 = current;

	render(accumulate / targetFrametime);
}
