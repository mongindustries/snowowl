//
// Created by Michael Ong on 7/4/20.
//
#pragma once

#ifndef VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#endif

#if defined(SWL_DARWIN)
#define VK_USE_PLATFORM_MACOS_MVK
#elif defined(SWL_WIN32)
#define VK_VK_USE_PLATFORM_WIN32_KHR
#endif

#include <vulkan/vulkan.hpp>
