//
// Created by micha on 4/15/2020.
//
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <libloaderapi.h>

#include "file_manager.hpp"

using namespace swl::cx;

using namespace std;
using namespace std::filesystem;

path resourcePathFromModule() {
	wstring result;
	result.resize(512);

	GetModuleFileName(nullptr, (LPWSTR) result.data(), result.size());

	return path(result).remove_filename();
}

path file_manager::resourcePath = resourcePathFromModule();
