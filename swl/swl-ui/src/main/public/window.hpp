//
// Created by Michael Ong on 31/3/20.
//
#pragma once

#include <vector>
#include <string>

#include <headerconv.hpp>
#include <hidden_implem.hpp>
#include <point.hpp>
#include <rect.hpp>
#include <core.hpp>

#include "event.hpp"
#include "windowSurface.hpp"

SNOW_OWL_NAMESPACE(ui::backend)

namespace backend { struct WindowBackend; }

SNOW_OWL_NAMESPACE_END

SNOW_OWL_NAMESPACE(ui)

struct SWL_EXPORT Application;

struct SWL_EXPORT Window {

	enum class State {
		Active,
		Paused,
		Background
	};

	Window();

	Window(std::string window_name, const cx::Rect &frame);

	// window properties

	void
		setTitle      (const std::string &new_title);

	void
		setFrame      (const cx::Rect &new_frame);


	[[nodiscard]] cx::DriverHandle
		getHandle     () const { return _handle; }

	[[nodiscard]] WindowSurface
		getSurface    () const;

	// window events

	void
		addEventClose (const Event<void, const Window&> &event);


	bool
		operator<     (const Window &rhs) const;

	bool
		operator==    (const Window &rhs) const;

	friend struct backend::WindowBackend;

private:

	typedef std::vector<Event<void, const Window&>> EventCloseList;

	std::string      _title{};
	cx::Rect         _frame{};

	cx::DriverHandle _handle{0};

	EventCloseList   _event_close_list;
};

SNOW_OWL_NAMESPACE_END
