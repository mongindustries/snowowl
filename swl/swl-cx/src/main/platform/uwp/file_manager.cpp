#include "file_manager.hpp"

#include <winrt/Windows.ApplicationModel.h>

SNOW_OWL_NAMESPACE(cx)

std::filesystem::path get_path() {

  auto package = winrt::Windows::ApplicationModel::Package::Current();

  return std::wstring{ package.InstalledLocation().Path() };
}


std::filesystem::path file_manager::resourcePath = get_path();

SNOW_OWL_NAMESPACE_END
