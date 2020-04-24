//
// Created by Michael Ong on 12/4/20.
//
#pragma once

#include <thread>
#include <chrono>

#include "header.hpp"

SNOW_OWL_NAMESPACE(cx)

struct game_loop {

	game_loop           (
		uint16_t targetFramerate,
		uint16_t bailAmount);

	virtual ~game_loop  () = default;


	void          open      ();

	void          close     ();

	void          frame     ();


	virtual void  preFrame  () = 0;

	virtual void  create    () = 0;

	virtual void  update    (std::chrono::milliseconds delta) = 0;

	virtual void  render    (float offset) = 0;

private:

	std::thread               game_thread;

	std::chrono::milliseconds t1{};
	std::chrono::milliseconds accumulate{};

	bool                      running{false};

	std::chrono::milliseconds target_frame_time{};
	uint16_t                  target_update_count{};
};

SNOW_OWL_NAMESPACE_END
