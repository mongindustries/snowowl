cmake_minimum_required(VERSION 3.16.0)

file(READ ../../../local.properties PROPS)
string(REGEX MATCHALL [[vulkan\.sdk=((\/[a-zA-Z0-9._-]+)+)]] PROP_LIST ${PROPS})

set(VULKAN_SDK ${CMAKE_MATCH_1})

set(HEADER_LIST
    public/graphics.hpp
    public/screen.hpp
    public/surface.hpp public/graphicsContext.hpp public/graphicsSwapchain.hpp public/graphicsBuffer.hpp)

set(PRIVATE_HEADER_LIST
    headers/swl_vulkan.hpp)

set(SOURCE_LIST
    cpp/graphics.cpp
    cpp/surface.cpp)

if(WIN32)
    set(PLATFORM_SOURCE_LIST)

    set(PLATFORM_HEADER_LIST)
endif()

if(APPLE)
    set(PLATFORM_SOURCE_LIST)

    set(PLATFORM_HEADER_LIST)
endif()

add_library(gx
    ${SOURCE_LIST}
    ${HEADER_LIST}
    ${PRIVATE_HEADER_LIST}
    ${PLATFORM_SOURCE_LIST}
    ${PLATFORM_HEADER_LIST})

target_include_directories(gx
    PUBLIC  public
    PRIVATE headers

    PRIVATE ../../../swl-cx/src/main/public
    PRIVATE ../../../swl-ui/src/main/public

    PRIVATE ../../../swl-ui/src/main/headers

    PRIVATE ${SRC_OS}
    PRIVATE ${VULKAN_SDK}/include)

target_link_libraries(gx
    PUBLIC cx
    PUBLIC ui)
