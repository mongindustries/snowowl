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

struct WindowBackend;

SNOW_OWL_NAMESPACE_END

SNOW_OWL_NAMESPACE(ui)


struct SWL_EXPORT Application {

	explicit Application(void* native_instance);

	Application(const Application& copy) = delete;

	Application& operator= (const Application& copy) = delete;
	

	Application(Application&& mov) noexcept = default;

	Application& operator= (Application&& move) noexcept = default;

	
	virtual ~Application() = default;

	// methods

	cx::exp::ptr<Window> createWindow(const std::string& caption, const cx::rect& frame);

	// hooks

	virtual void applicationCreate() {
	}

	virtual void applicationDestroy() {
	}

	template<typename App, std::enable_if_t< std::is_base_of_v<Application, App>, int > = 0>
	static int runApplication(App app) {

		App::preHeat(app);
		app.applicationCreate();
		App::runLoop(app);

		return 0;
	}

	friend struct backend::WindowBackend;

private:

	void* _native_instance;

	static void preHeat(Application &app);

	static void runLoop(Application &app);
};

SNOW_OWL_NAMESPACE_END
