#include "uwp_application.hpp"
#include "swl_window_backend.hpp"

#include <winrt/windows.applicationmodel.core.h>

using namespace winrt;

SNOW_OWL_NAMESPACE(ui::backend)

ApplicationSource* ApplicationSource::instance = nullptr;

ApplicationSource::ApplicationSource  (const cx::exp::ptr_ref<ui::application> &app): app(app), _window() { }

// IFVS

IFV ApplicationSource::CreateView     () {
  return *this;
}

  // IFV

void ApplicationSource::Initialize    (Windows::ApplicationModel::Core::CoreApplicationView const& app) {

  app.Activated([&](Windows::ApplicationModel::Core::CoreApplicationView const& view, Windows::ApplicationModel::Activation::IActivatedEventArgs const&) {

    auto &title = view.TitleBar();
    title.ExtendViewIntoTitleBar(true);

    view.CoreWindow().Activate();
  });
}

void ApplicationSource::Load          (hstring const&) {

}

void ApplicationSource::Uninitialize  () {

}

void ApplicationSource::SetWindow     (Windows::UI::Core::CoreWindow const& window) {

  _core_window = window;

  void* native{ nullptr };
  winrt::copy_to_abi(window, native);

  backend::window_backend::instance->create_native(_window.pointer(), native);
}

void ApplicationSource::Run           () {

  app->on_create();

  _core_window.get().Dispatcher().ProcessEvents(Windows::UI::Core::CoreProcessEventsOption::ProcessUntilQuit);

  app->on_destroy();
}

SNOW_OWL_NAMESPACE_END


SNOW_OWL_NAMESPACE(ui)

void  application::pre_heat(application& app) {

}

int   application::run_loop(application& app) {

  winrt::init_apartment();

  auto source                           = backend::ApplicationSource(cx::exp::ptr_ref{ &app });
  backend::ApplicationSource::instance  = &source;

  Windows::ApplicationModel::Core::CoreApplication::Run(source);

  return 0; // no run loop. handled by IFrameworkView::Run.
}

cx::exp::ptr_ref<ui::window> application::get_main_window() {
  return cx::exp::ptr_ref{ backend::ApplicationSource::instance->_window };
}

SNOW_OWL_NAMESPACE_END
