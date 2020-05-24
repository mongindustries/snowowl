#include "uwp_application.hpp"
#include <winrt/windows.applicationmodel.core.h>

using namespace winrt;

SNOW_OWL_NAMESPACE(ui::backend)

ApplicationSource* ApplicationSource::instance = nullptr;

ApplicationSource::ApplicationSource  (const cx::exp::ptr_ref<ui::application> &app): app(app), _window(nullptr) { }

// IFVS

IFV ApplicationSource::CreateView     () {
  return *this;
}

  // IFV

void ApplicationSource::Initialize    (Windows::ApplicationModel::Core::CoreApplicationView const& app) {

  app.Activated([&](Windows::ApplicationModel::Core::CoreApplicationView const& view, Windows::ApplicationModel::Activation::IActivatedEventArgs const&) {

    const auto title = view.TitleBar();
    title.ExtendViewIntoTitleBar(true);

    view.CoreWindow().Activate();
  });
}

void ApplicationSource::Load          (hstring const&) {

}

void ApplicationSource::Uninitialize  () {
  app->on_destroy();
}

void ApplicationSource::SetWindow     (Windows::UI::Core::CoreWindow const& window) {

  _core_window = window;

  void* native{ nullptr };
  copy_to_abi(window, native);

  _window = cx::exp::ptr<ui::window>(new ui::window("", cx::rect{ {}, {800, 480}}, native));
}

void ApplicationSource::Run           () {

  app->on_create();

  _core_window.get().Dispatcher().ProcessEvents(Windows::UI::Core::CoreProcessEventsOption::ProcessUntilQuit);
}

SNOW_OWL_NAMESPACE_END


SNOW_OWL_NAMESPACE(ui)

void  application::pre_heat(application& app) {

}

int   application::run_loop(application& app) {

  init_apartment();

  auto source                           = backend::ApplicationSource(cx::exp::ptr_ref{ &app });
  backend::ApplicationSource::instance  = &source;

  Windows::ApplicationModel::Core::CoreApplication::Run(source);

  return 0; // no run loop. handled by IFrameworkView::Run.
}

std::reference_wrapper < window >
  application::main_window() {
  return backend::ApplicationSource::instance->_window.get();
}

SNOW_OWL_NAMESPACE_END
