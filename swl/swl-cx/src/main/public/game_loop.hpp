//
// Created by Michael Ong on 12/4/20.
//
#pragma once

#include <thread>
#include <chrono>

#include "header.hpp"

SNOW_OWL_NAMESPACE(cx)

struct GameLoop {

	GameLoop(uint16_t targetFramerate, uint16_t bailAmount);

	virtual ~GameLoop() = default;


	void open();

	void close();


	virtual void create() = 0;

	virtual void update(std::chrono::milliseconds delta) = 0;

	virtual void render(float offset) = 0;

private:

	std::thread game_thread;

    bool running{false};

    std::chrono::milliseconds targetFrametime{};
    uint16_t maxUpdateCount{};
};

SNOW_OWL_NAMESPACE_END
