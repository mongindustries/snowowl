cmake_minimum_required(VERSION 3.16.0)

set(HEADER_LIST
        public/application.hpp
        public/event.hpp
        public/window.hpp)

set(PRIVATE_HEADER_LIST)

set(SOURCE_LIST
        cpp/application.cpp
    )

if(WIN32)
    if(WINDOWS_STORE)

      set(PLATFORM_SOURCE_LIST
            platform/uwp/uwp_application.cpp
            platform/uwp/uwp_window.cpp)

      set(PLATFORM_HEADER_LIST
            platform/uwp/uwp_application.hpp
            platform/uwp/uwp_internal_state.h)

    else()
      set(PLATFORM_SOURCE_LIST
            platform/windows/swl_application.cpp
            platform/windows/swl_window.cpp)

      set(PLATFORM_HEADER_LIST
            latform/windows/swl_internal_state.h)
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
