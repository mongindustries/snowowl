#include <header.hpp>
#include <ownership.hpp>

#include "application.hpp"

#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.UI.Core.h>

SNOW_OWL_NAMESPACE(ui::backend)

using IFVS  = winrt::Windows::ApplicationModel::Core::IFrameworkViewSource;
using IFV   = winrt::Windows::ApplicationModel::Core::IFrameworkView;

struct ApplicationSource : winrt::implements<ApplicationSource, IFVS, IFV> {

  static ApplicationSource* instance;

  cx::exp::ptr_ref<ui::application> app;

  ApplicationSource (const cx::exp::ptr_ref<ui::application>& app);

  // IFVS

  IFV   CreateView  ();

  // IFV

  void  Initialize  (winrt::Windows::ApplicationModel::Core::CoreApplicationView const& app);

  void  Load        (winrt::hstring const&);

  void  Uninitialize();

  void  SetWindow   (winrt::Windows::UI::Core::CoreWindow const& window);

  void  Run         ();


  cx::exp::ptr_ref<ui::application>
    _app;

  cx::exp::ptr<ui::window>
    _window;

  winrt::agile_ref<winrt::Windows::UI::Core::CoreWindow>
    _core_window;
};

SNOW_OWL_NAMESPACE_END
