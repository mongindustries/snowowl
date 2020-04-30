cmake_minimum_required(VERSION 3.16.0)

set(HEADER_LIST
        public/application.hpp
        public/event.hpp
        public/window.hpp
        public/window_surface.hpp)

set(PRIVATE_HEADER_LIST
        headers/swl_window_backend.hpp
        headers/swl_window_sink.hpp)

set(SOURCE_LIST
        cpp/application.cpp
        cpp/window.cpp
        cpp/window_surface.cpp

        headers/swl_window_sink.cpp)

if(WIN32)
    if(WINDOWS_STORE)
      set(PLATFORM_SOURCE_LIST
              platform/uwp/uwp_application.cpp
              platform/uwp/uwp_window_backend.cpp)
    else()
      set(PLATFORM_SOURCE_LIST
              platform/windows/swl_win32_app.cpp
              platform/windows/swl_win32_window.cpp
              platform/windows/swl_window_backend.cpp)

      set(PLATFORM_HEADER_LIST
              platform/windows/swl_win32_window.hpp)
    endif()
endif()

if(APPLE)
    set(PLATFORM_SOURCE_LIST
            platform/darwin/swl_application.mm
            platform/darwin/swl_window.mm
            platform/darwin/swl_window_backend.mm
            platform/darwin/swl_view.mm
            platform/darwin/swl_viewController.mm)

    set(PLATFORM_HEADER_LIST
            platform/darwin/swl_application.hxx
            platform/darwin/swl_window.hxx
            platform/darwin/swl_view.hxx
            platform/darwin/swl_viewController.hxx)
endif()

add_library(ui
        ${SOURCE_LIST}
        ${HEADER_LIST}
        ${PRIVATE_HEADER_LIST}
        ${PLATFORM_SOURCE_LIST}
        ${PLATFORM_HEADER_LIST})

target_include_directories(ui
    PUBLIC  public
    PRIVATE headers

    PRIVATE ../../../swl-cx/src/main/public
    PRIVATE ../../../swl-gx/src/main/public

    PRIVATE ${SRC_OS}
    PRIVATE ${VULKAN_SDK}/include)

target_link_libraries(ui
    PUBLIC cx)
