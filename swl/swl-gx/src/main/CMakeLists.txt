cmake_minimum_required(VERSION 3.16.0)

set(HEADER_LIST
    public/graphics.hpp
    public/graphicsContext.hpp
    public/graphicsSwapChain.h
    public/vulkanImport.h
    public/vulkanGraphicsContext.hpp
    public/vulkanGraphicsSwapChain.hpp public/vulkanGraphicsQueue.hpp)

set(PRIVATE_HEADER_LIST
    headers/vulkanGraphicsBackend.hpp)

set(SOURCE_LIST
    cpp/graphics.cpp
    cpp/vulkanGraphicsContext.cpp
    cpp/vulkanGraphicsSwapChain.cpp cpp/vulkanGraphicsQueue.cpp)

if(WIN32)
    set(PLATFORM_SOURCE_LIST 
        platform/windows/vulkanGraphicsBackend.cpp)

    set(PLATFORM_HEADER_LIST)
endif()

if(APPLE)
    set(PLATFORM_SOURCE_LIST
        platform/darwin/vulkanGraphicsBackend.cpp)

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
    PRIVATE ../../../swl-cx/src/main/headers

    PRIVATE ../../../swl-ui/src/main/public
    PRIVATE ../../../swl-ui/src/main/headers

    PRIVATE ${SRC_OS}
    PRIVATE ${VULKAN_SDK}/include)

target_link_libraries(gx
    PUBLIC cx
    PUBLIC ui)
