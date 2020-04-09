cmake_minimum_required(VERSION 3.16.0)

set(APP_SOURCES
    cpp/app.cpp
    cpp/renderer.cpp
    headers/renderer.hpp
    cpp/rendererQueue.cpp
    headers/rendererQueue.hpp)

if(WIN32)
    add_executable(app WIN32
            ${APP_SOURCES}
            platform/windows/app.rc
            platform/windows/resource.h)
endif()

if(APPLE)
    add_executable(app MACOSX_BUNDLE
            ${APP_SOURCES})

    set_target_properties(app PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/platform/darwin/Info.plist.in)

    target_link_libraries(app
            "-framework Foundation"
            "-framework QuartzCore"
            "-framework AppKit"
            "-framework Metal")

endif()

target_include_directories(app
    PUBLIC headers
    PUBLIC ../../../swl-cx/src/main/public
    PUBLIC ../../../swl-ui/src/main/public
    PUBLIC ../../../swl-gx/src/main/public
    PUBLIC ${VULKAN_SDK}/include)

target_link_libraries(app
    cx
    ui
    gx)
