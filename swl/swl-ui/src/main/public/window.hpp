//
// Created by Michael Ong on 31/3/20.
//
#pragma once

#include <vector>
#include <string>
#include <condition_variable>

#include <header.hpp>
#include <core.hpp>
#include <rect.hpp>
#include <ownership_exp.hpp>
#include <game_loop.hpp>

#include "event.hpp"

SNOW_OWL_NAMESPACE(gx)

struct graphics_swap_chain;

SNOW_OWL_NAMESPACE_END

SNOW_OWL_NAMESPACE(ui)

struct application;


struct window_sink;

struct window_surface;


struct SWL_EXPORT window final {

	enum class state {
		Active,
		Paused,
		Background
	};

	window            (const window&) = delete;

	window& operator= (const window&) = delete;


	window            (window&&) noexcept;

	window& operator= (window&&) noexcept;


	window  ();

	window  (std::string window_name, const cx::rect &frame);

	~window ();

	// window properties

	void  bind_loop     (const cx::exp::ptr_ref<cx::game_loop> &game_loop);

	void  set_title     (const std::string &value);

	void  set_frame     (const cx::rect &value);

	[[nodiscard]] std::string
				get_title     () const;

	[[nodiscard]] cx::size_2d
				get_size      () const;

	[[nodiscard]] cx::rect
				get_frame     () const;

	[[nodiscard]] cx::exp::ptr_ref<window_sink>
				get_sink      () const;

	[[nodiscard]] bool
				is_sizing     () const;

	// window events

	bool  operator<     (const window &rhs) const;

	bool  operator==    (const window &rhs) const;

private:

	typedef std::vector<event<void, const window&>>                   EventCloseList;

	typedef std::vector<event<void, const window&, const state&>>     EventStateList;

	typedef std::vector<event<void, const window&, const cx::rect&>>  EventSizeList;


	cx::driver_handle         handle{ 0 };

	std::string               title{};

	cx::rect                  frame{};

	bool                      resizing{ false };


	cx::exp::ptr<window_sink> sink;

	friend struct window_sink;
	friend struct window_surface;

public:

	cx::exp::ptr_ref<gx::graphics_swap_chain> swap_chain;

	cx::exp::ptr_ref<cx::game_loop>           game_loop;

	EventCloseList          event_on_close;
	EventStateList          event_on_state;
	EventSizeList           event_on_resize;
};

SNOW_OWL_NAMESPACE_END
