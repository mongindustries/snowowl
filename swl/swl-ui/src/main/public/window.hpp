//
// Created by Michael Ong on 31/3/20.
//
#pragma once

#include <vector>
#include <string>
#include <condition_variable>

#include <header.hpp>
#include <rect.hpp>
#include <core.hpp>
#include <hidden_implem.hpp>

#include "event.hpp"

SNOW_OWL_NAMESPACE(ui)

struct Application;


struct WindowSink;

struct WindowSurface;


struct SWL_EXPORT Window final {

	enum class State {
		Active,
		Paused,
		Background
	};

	std::condition_variable waitForNoWindowResizing;
	std::mutex              lockForNoWindowResizing;
	
	Window  ();

	Window  (std::string window_name, const cx::Rect &frame);

	// window properties

	void
		setTitle      (const std::string &new_title);

	void
		setFrame      (const cx::Rect &new_frame);

	[[nodiscard]] std::string
		getTitle      () const;

	[[nodiscard]] cx::Size2D
		getSize       () const;

	[[nodiscard]] cx::Rect
		getFrame      () const;

	[[nodiscard]] WindowSink*
		getSink       () const;

	[[nodiscard]] bool
		isSizing      () const;

	// window events

	bool
		operator<     (const Window &rhs) const;

	bool
		operator==    (const Window &rhs) const;

private:

	typedef std::vector<Event<void, const Window&>> EventCloseList;

	typedef std::vector<Event<void, const Window&, const cx::Rect&>> EventSizeList;

	typedef std::vector<Event<void, const Window&, const State&>> EventStateList;


	cx::DriverHandle _handle{0};

	std::string      _title{};
	cx::Rect         _frame{};

	WindowSink*      _sink;

	bool             _resizing{ false };

	friend struct WindowSink;
	friend struct WindowSurface;

public:

	EventCloseList   _event_close_list;
	EventSizeList    _event_size_list;
	EventStateList   _event_state_list;
};

SNOW_OWL_NAMESPACE_END
