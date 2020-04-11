cmake_minimum_required(VERSION 3.16.0)

set(VERTEX_SOURCES
    hlsl/simple.vert)

set(FRAGMENT_SOURCES
    hlsl/simple.frag)

set(APP_SOURCES
    cpp/app.cpp
    cpp/renderer.cpp
    headers/renderer.hpp
    cpp/rendererQueue.cpp
    headers/rendererQueue.hpp
    cpp/rendererSwapchain.cpp
    headers/rendererSwapchain.hpp
    cpp/rendererBuffer.cpp
    headers/rendererBuffer.hpp
    cpp/rendererBufferBasicClear.cpp
    headers/rendererBufferBasicClear.hpp)

if(WIN32)

    set_property(SOURCE ${VERTEX_SOURCES}
        PROPERTY VS_TOOL_OVERRIDE "${CMAKE_SOURCE_DIR}\\../dxc\\dxc.exe -T vs_6_0 -spirv")
    set_property(SOURCE ${FRAGMENT_SOURCES}
        PROPERTY VS_TOOL_OVERRIDE "${CMAKE_SOURCE_DIR}\\..\\dxc\\dxc.exe -T ps_6_0 -spirv")

    add_executable(app WIN32
            ${APP_SOURCES}
            ${VERTEX_SOURCES}
            ${FRAGMENT_SOURCES}
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
