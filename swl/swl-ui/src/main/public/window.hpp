//
// Created by Michael Ong on 31/3/20.
//
#pragma once

#include <vector>
#include <string>

#include <headerconv.hpp>
#include <point.hpp>
#include <rect.hpp>
#include <hidden_implem.hpp>

#include "event.hpp"

SNOW_OWL_NAMESPACE(ui)

struct Window {

	enum class State {
		active,
		paused,
		background
	};

	Window(const std::string &window_name, const cx::Size2D &size);

	Window(const std::string &window_name, const cx::Rect &frame);

	// window properties

	void setName(const std::string &name);

	void setFrame(const cx::Rect &frame);

	// window events

	void addEventFrame(Event<void, const Window&, cx::Rect> event);

	void addEventActiveState(Event<void, const Window&, State> event);

	void addEventClose(Event<void, const Window&> event);

private:

	typedef std::vector<Event<void, const Window&, cx::Rect>> EventFrameList;
	typedef std::vector<Event<void, const Window&, State>>    EventActiveState;

	EventFrameList    event_frame;
	EventActiveState  event_activateState;
};

SNOW_OWL_NAMESPACE_END
