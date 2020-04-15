//
// Created by Michael Ong on 12/4/20.
//

#include <thread>
#include <chrono>

#include "game_loop.hpp"

using namespace std;
using namespace chrono;
using namespace swl::cx;

GameLoop::GameLoop(uint16_t targetFramerate, uint16_t bailAmount)
: targetFrametime(1000 / targetFramerate),
maxUpdateCount(bailAmount) {


}

void GameLoop::open() {

	auto func = [](GameLoop* game_loop, milliseconds frametime, uint16_t maxUpdateCount) {

		game_loop->create();

		milliseconds t1 = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());

		milliseconds accumulate{};
		while (game_loop->running) {

			milliseconds current    = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
			milliseconds offset     = current - t1;

			accumulate += offset;

			while (accumulate >= frametime) {
				game_loop->update(frametime);
				accumulate -= frametime;
			}

			t1 = current;

			game_loop->render(accumulate / frametime);
		}
	};

	running = true;

	game_thread = thread(func, this, targetFrametime, maxUpdateCount);
	game_thread.detach();
}

void GameLoop::close() {

	running = false;
}
