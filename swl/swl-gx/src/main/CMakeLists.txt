cmake_minimum_required(VERSION 3.16.0)

set(HEADER_LIST
        public/buffer.hpp
        public/buffer_allocator.hpp
        public/context.hpp
        public/factory.hpp
        public/pipeline.hpp
        public/render_block.hpp
        public/render_pass.hpp
        public/render_pipeline.hpp
        public/resource_reference.hpp
        public/queue.hpp
        public/shader.hpp
        public/swap_chain.hpp
        public/transfer_block.hpp)

set(PRIVATE_HEADER_LIST)

set(SOURCE_LIST
        cpp/buffer_allocator.cpp
        cpp/context.cpp
        cpp/render_pass.cpp
        cpp/render_block.cpp
        cpp/render_pipeline.cpp
        cpp/queue.cpp
        cpp/swap_chain.cpp
        cpp/transfer_block.cpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/cpp       PREFIX source/cpp FILES ${SOURCE_LIST})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/public    PREFIX source/hpp FILES ${HEADER_LIST})

if(WIN32)
    set(PLATFORM_SOURCE_LIST
        "platform/windows/directx/buffer.cpp"
        "platform/windows/directx/buffer_allocator.cpp"
        "platform/windows/directx/context.cpp"
        "platform/windows/directx/factory.cpp"
        "platform/windows/directx/pipeline.cpp"
        "platform/windows/directx/queue.cpp" 
        "platform/windows/directx/render_pass.cpp"
        "platform/windows/directx/render_pipeline.cpp"
        "platform/windows/directx/render_block.cpp"
        "platform/windows/directx/resource_reference.cpp"
        "platform/windows/directx/swap_chain.cpp"
        "platform/windows/directx/transfer_block.cpp")

    set(PLATFORM_HEADER_LIST
        "public/directx/buffer.h"
        "public/directx/buffer_allocator.h"
        "public/directx/context.h"
        "public/directx/factory.h"
        "public/directx/pipeline.h"
        "public/directx/queue.h"
        "public/directx/render_pass.h"
        "public/directx/render_pipeline.h"
        "public/directx/render_block.h"
        "public/directx/resource_reference.h"
        "public/directx/swap_chain.h"
        "public/directx/transfer_block.h")

    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/platform/windows/directx PREFIX directx/cpp FILES ${PLATFORM_SOURCE_LIST})
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/public/directx           PREFIX directx/hpp FILES ${PLATFORM_HEADER_LIST})
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
