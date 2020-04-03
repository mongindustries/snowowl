//
// Created by micha on 4/3/2020.
//
#pragma once

#include <string>
#include <vector>
#include <type_traits>

#include <headerconv.hpp>
#include <ownership.hpp>

#include "window.hpp"

SNOW_OWL_NAMESPACE(ui)

struct SWL_EXPORT Application {

	explicit Application(void* nativeInstance);

	// methods

	// hooks

	virtual void applicationCreate() {
	}

	virtual void applicationDestroy() {
	}

private:

	void* nativeInstance;

	cx::Own<Window> window;

	std::vector<cx::Own<Window>> windowList;

	static void RunLoop(Application &app);
};


template<typename App, std::enable_if_t< std::is_base_of_v<Application, App>, int > = 0>
void runApplication(App app) {

	app.applicationCreate();

	App::RunLoop(app);

	app.applicationDestroy();
}

SNOW_OWL_NAMESPACE_END
