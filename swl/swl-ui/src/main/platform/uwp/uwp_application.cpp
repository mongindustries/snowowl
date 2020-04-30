#include "application.hpp"

#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.UI.Core.h>

using namespace winrt;

SNOW_OWL_NAMESPACE(ui)

using IFVS  = Windows::ApplicationModel::Core::IFrameworkViewSource;
using IFV   = Windows::ApplicationModel::Core::IFrameworkView;

struct ApplicationSource : implements<ApplicationSource, IFVS, IFV> {

  // IFVS

  IFV CreateView      () {
    return *this;
  }

  // IFV

  void Initialize     (Windows::ApplicationModel::Core::CoreApplicationView const& app) {

    app.Activated([&](Windows::ApplicationModel::Core::CoreApplicationView const& view, Windows::ApplicationModel::Activation::IActivatedEventArgs const&) {
      Windows::UI::Core::CoreWindow::GetForCurrentThread().Activate();
    });
  }

  void Load           (hstring const&) {

  }

  void Uninitialize   () {

  }

  void SetWindow      (Windows::UI::Core::CoreWindow const& window) {

    _window = window;

    window.SizeChanged([](Windows::UI::Core::CoreWindow const& window, Windows::UI::Core::WindowSizeChangedEventArgs const& args) {
    });
  }

  void Run            () {
    _window.get().Dispatcher().ProcessEvents(Windows::UI::Core::CoreProcessEventsOption::ProcessUntilQuit);
  }

  agile_ref<Windows::UI::Core::CoreWindow> _window;

private:
};


void  application::pre_heat(application& app) {

}

int   application::run_loop(application& app) {

  Windows::ApplicationModel::Core::CoreApplication::Run(ApplicationSource());

  return 0; // no run loop. handled by IFrameworkView::Run.
}

SNOW_OWL_NAMESPACE_END
