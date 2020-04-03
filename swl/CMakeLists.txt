cmake_minimum_required(VERSION 3.16.0)

project(snow_owl VERSION 0.1.0 LANGUAGES CXX)

# c++ stuff

set(CMAKE_CXX_STANDARD          20  )
set(CMAKE_CXX_STANDARD_REQUIRED ON  )
set(CMAKE_CXX_EXTENSIONS        OFF )

# variables

set(SRC_ROOT "src/main/cpp")

set(PRI_HEAD "src/main/headers")
set(PUB_HEAD "src/main/public")

if (WIN32)

set(SRC_OS "src/main/platform/windows")
add_compile_definitions(SWL_WIN32=1)

endif (WIN32)

if (APPLE)

set(SRC_OS "src/main/platform/darwin")
add_compile_definitions(SWL_DARWIN=1)

endif (APPLE)

add_subdirectory(swl-cx)
add_subdirectory(swl-ui)
add_subdirectory(swl-gx)
