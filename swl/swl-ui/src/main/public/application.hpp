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

	/**
	 * Creates a new window. Window reference is held by this instance.
	 * @param caption The caption for the window.
	 * @param frame Initial frame for the window. For OS that requires a fullscreen window, this param is ignored.
	 * @return A mutable reference <code>MutateOwn</code> that has the reference to the created window.
	 */
	cx::MutateOwn<ui::Window> createWindow(const std::string &caption, const cx::Rect &frame);

	// hooks

	virtual void applicationCreate() {
	}

	virtual void applicationDestroy() {
	}

	template<typename App, std::enable_if_t< std::is_base_of_v<Application, App>, int > = 0>
	static unsigned int runApplication(App app) {

		App::PreHeat(app);
		app.applicationCreate();
		App::RunLoop(app);

		return 0;
	}

	friend struct backend::WindowBackend;

private:

	void* nativeInstance;

	std::vector<cx::Own<Window>> windowList;


	static void PreHeat(Application &app);

	static void RunLoop(Application &app);
};

SNOW_OWL_NAMESPACE_END
