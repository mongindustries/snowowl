cmake_minimum_required(VERSION 3.16.0)

set(VERTEX_SOURCES
    hlsl/simple.vert)

set(FRAGMENT_SOURCES
    hlsl/simple.frag)

set(APP_SOURCES
    ${FRAGMENT_SOURCES}
    ${VERTEX_SOURCES}
    cpp/app.cpp
        cpp/world.cpp
        headers/world.hpp)

if(WIN32)
    add_executable(app WIN32
            ${APP_SOURCES}
            platform/windows/app.rc
            platform/windows/resource.h)

    target_link_libraries(app
        "dwmapi")
endif()

if(APPLE)
    add_executable(app MACOSX_BUNDLE
        ${APP_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/platform/darwin/app.xcassets)

    set_target_properties(app PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/platform/darwin/Info.plist.in
        XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME AppIcon)

    set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/platform/darwin/app.xcassets
        PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

    target_link_libraries(app
            c++
            "-framework Foundation"
            "-framework QuartzCore"
            "-framework AppKit"
            "-framework Metal"
            "-framework IOKit"
            "-framework IOSurface")
endif()

if(DEFINED ${VULKAN_SDK})
    get_shader_files(app)
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
