cmake_minimum_required(VERSION 3.16.0)

set(HEADER_LIST
        public/graphics_context.hpp
        public/graphics_swap_chain.hpp
        public/graphics_queue.hpp
        public/graphics_render_pass.hpp
        public/graphics_shader.hpp
        public/graphics_buffer.hpp
        public/graphics_sampler.hpp
        public/graphics_render_pipeline.hpp
        public/graphics_buffer_allocator.hpp 
        public/graphics_render_block.hpp
        public/graphics_resource_reference.hpp)

set(PRIVATE_HEADER_LIST)

set(SOURCE_LIST
        cpp/graphics_context.cpp
        cpp/graphics_swap_chain.cpp
        cpp/graphics_render_pipeline.cpp
        cpp/graphics_queue.cpp
        cpp/graphics_render_pass.cpp cpp/graphics_buffer_allocator.cpp
        cpp/graphics_render_block.cpp)

if(WIN32)
    set(PLATFORM_SOURCE_LIST
            "platform/windows/directx/context.cpp"
            "platform/windows/directx/queue.cpp" 
            "platform/windows/directx/swap_chain.cpp"
            "platform/windows/directx/pipeline.cpp"
            "platform/windows/directx/render_block.cpp"
            "platform/windows/directx/resource_reference.cpp")

    set(PLATFORM_HEADER_LIST
            "public/directx/context.h"
            "public/directx/queue.h"
            "public/directx/swap_chain.h"
            "public/directx/pipeline.h"
            "public/directx/render_block.h"
            "public/directx/resource_reference.h")

    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX directx/cpp FILES ${PLATFORM_SOURCE_LIST})
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX directx/hpp FILES ${PLATFORM_HEADER_LIST})
endif()

if(APPLE)
    set(PLATFORM_SOURCE_LIST
            platform/darwin/metal/mtl_swap_chain.mm
            platform/darwin/metal/mtl_queue.mm
            platform/darwin/metal/mtl_render_pipeline.mm
            platform/darwin/metal/mtl_context.mm
            platform/darwin/metal/mtl_render_block.mm
            platform/darwin/metal/mtl_buffer.mm
            platform/darwin/metal/mtl_cpu_fence.mm)

    set(PLATFORM_HEADER_LIST
            public/metal/mtl_context.h
            public/metal/mtl_swap_chain.h
            public/metal/mtl_queue.h
            public/metal/mtl_render_pass.h
            public/metal/mtl_render_pipeline.h
            public/metal/mtl_buffer.h
            public/metal/mtl_render_block.h
            public/metal/mtl_cpu_fence.h)

    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX metal FILES ${PLATFORM_SOURCE_LIST} ${PLATFORM_HEADER_LIST})
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
