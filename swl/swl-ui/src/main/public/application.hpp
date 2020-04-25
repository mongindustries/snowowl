//
// Created by micha on 4/3/2020.
//
#pragma once

#include <string>
#include <vector>
#include <type_traits>

#include <header.hpp>
#include <ownership_exp.hpp>

#include "window.hpp"

SNOW_OWL_NAMESPACE(ui::backend)

struct window_backend;

SNOW_OWL_NAMESPACE_END

SNOW_OWL_NAMESPACE(ui)

struct SWL_EXPORT application {

	application             (const application&) = delete;

	application& operator=  (const application&) = delete;


	application             (application&& mov) noexcept  = default;

	application& operator=  (application&& move) noexcept = default;


	explicit application    (void* native_instance);

	virtual ~application    () = default;

	// methods

	cx::exp::ptr<window>
			create_window       (const std::string& caption, const cx::rect& frame);

	// hooks

	virtual void
			on_create           () {
	}

	virtual void
			on_destroy          () {
	}

	template<typename App, std::enable_if_t< std::is_base_of_v<application, App>, int > = 0>
	static int runApplication(App app) {

		App::preHeat(app);
		app.applicationCreate();

		return App::runLoop(app);
	}

	friend struct backend::window_backend;

private:

	void* native_instance;

	static void
			pre_heat            (application &app);

	static int
			run_loop            (application &app);
};

SNOW_OWL_NAMESPACE_END
