cmake_minimum_required(VERSION 3.16.0)

set(HEADER_LIST
    public/graphics.hpp
    public/graphics_context.hpp
    public/graphics_swap_chain.h
        public/vulkan_import.h
        public/vulkan_context.hpp
        public/vulkan_queue.hpp
        public/vulkan_swap_chain.hpp
    public/vulkan_shader.hpp public/vulkan_fence_lock.hpp public/vulkan_buffer_recorder.hpp)

set(PRIVATE_HEADER_LIST
        headers/vulkan_graphics_backend.hpp)

set(SOURCE_LIST
    cpp/graphics.cpp
        cpp/vulkan_context.cpp
        cpp/vulkan_queue.cpp
        cpp/vulkan_swap_chain.cpp cpp/vulkan_shader.cpp cpp/vulkan_fence_lock.cpp)

if(WIN32)
    set(PLATFORM_SOURCE_LIST
            platform/windows/vulkan_graphics_backend.cpp)

    set(PLATFORM_HEADER_LIST)
endif()

if(APPLE)
    set(PLATFORM_SOURCE_LIST
            platform/darwin/vulkan_graphics_backend.cpp)

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
