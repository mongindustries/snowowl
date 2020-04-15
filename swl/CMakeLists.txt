cmake_minimum_required(VERSION 3.16.0)

if(APPLE)
project(snow_owl VERSION 0.1.0 LANGUAGES CXX OBJC)
endif()

if(WIN32)
project(snow_owl VERSION 0.1.0 LANGUAGES CXX)
endif()

# c++ stuff

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD          20  )
set(CMAKE_CXX_STANDARD_REQUIRED ON  )
set(CMAKE_CXX_EXTENSIONS        OFF )

# variables

set(SRC_ROOT "cpp")

set(PRI_HEAD "headers")
set(PUB_HEAD "public")

include(buildDir/compileShaders.cmake)

include(buildDir/importVulkan.cmake)
include(buildDir/applyPlatformFlags.cmake)

import_vulkan(${CMAKE_SOURCE_DIR}/local.properties)
apply_platform_flags()

option(PACKAGE_TESTS "Build tests" ON)

# platform files and macros

add_subdirectory(swl-cx/src/main)
add_subdirectory(swl-ui/src/main)
add_subdirectory(swl-gx/src/main)
add_subdirectory(app/src/main)

include(buildDir/addTesting.cmake)
