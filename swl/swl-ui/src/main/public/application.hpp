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

SNOW_OWL_NAMESPACE(ui::backend)

struct WindowBackend;

SNOW_OWL_NAMESPACE_END

SNOW_OWL_NAMESPACE(ui)


struct SWL_EXPORT Application {

	explicit Application(void* native_instance);

	Application(const Application& copy) = delete;

	Application& operator= (const Application& copy) = delete;
	

	Application(Application&& mov) noexcept;

	Application& operator= (Application&& move) noexcept;

	
	virtual ~Application() = default;

	// methods

	/**
	 * Creates a new window. Window reference is held by this instance.
	 * @param caption The caption for the window.
	 * @param frame Initial frame for the window. For OS that requires a fullscreen window, this param is ignored.
	 * @return A mutable reference <code>MutateOwn</code> that has the reference to the created window.
	 */
	cx::MutableBorrow<Window> createWindow(const std::string& caption, const cx::Rect& frame);

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

	static cx::DriverHandle windowFromNativeHandle(void* native_handle);

	static Window& windowWithHandle(cx::DriverHandle handle);

	friend struct backend::WindowBackend;
	
private:

	void* _native_instance;

	std::vector<cx::Own<Window>> _window_list;

	static void preHeat(Application &app);

	static void runLoop(Application &app);
};

SNOW_OWL_NAMESPACE_END
