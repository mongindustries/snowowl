cmake_minimum_required(VERSION 3.16.0)

set(HEADER_LIST
        public/graphics_context.hpp
        public/graphics_swap_chain.hpp
        public/graphics_queue.hpp
        public/graphics_pass.hpp
        public/graphics_shader.hpp
        public/graphics_buffer.hpp
        public/graphics_sampler.hpp
        public/graphics_pipeline.hpp)

set(PRIVATE_HEADER_LIST)

set(SOURCE_LIST
        cpp/graphics_context.cpp
        cpp/graphics_swap_chain.cpp
        cpp/graphics_pipeline.cpp
        cpp/graphics_queue.cpp
        cpp/graphics_pass.cpp)

if(WIN32)
    set(PLATFORM_SOURCE_LIST
            "platform/windows/directx/context.cpp"
            "platform/windows/directx/queue.cpp" 
            "platform/windows/directx/swap_chain.cpp" "platform/windows/directx/pipeline.cpp")

    set(PLATFORM_HEADER_LIST
            "public/directx/context.h" "public/directx/queue.h" "public/directx/swap_chain.h" "public/directx/pipeline.h")

    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX directx FILES ${PLATFORM_SOURCE_LIST} ${PLATFORM_HEADER_LIST})
endif()

if(APPLE)
    set(PLATFORM_SOURCE_LIST
            platform/darwin/metal/mtl_swap_chain.mm
            platform/darwin/metal/mtl_queue.mm
            platform/darwin/metal/mtl_pipeline.mm
            platform/darwin/metal/mtl_context.mm)

    set(PLATFORM_HEADER_LIST
            public/metal/mtl_context.h
            public/metal/mtl_swap_chain.h
            public/metal/mtl_queue.h
            public/metal/mtl_pass.h
            public/metal/mtl_pipeline.h)

    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX metal FILES ${PLATFORM_SOURCE_LIST} ${PLATFORM_HEADER_LIST})
endif()

add_library(gx
    ${SOURCE_LIST}
    ${HEADER_LIST}
    ${PRIVATE_HEADER_LIST}
    ${PLATFORM_SOURCE_LIST}
    ${PLATFORM_HEADER_LIST} "platform/windows/directx/swap_chain.cpp" "public/directx/pipeline.h" "platform/windows/directx/pipeline.cpp")

target_include_directories(gx
    PUBLIC  public
    PRIVATE headers

    PRIVATE ../../../swl-cx/src/main/public
    PRIVATE ../../../swl-ui/src/main/public

    PRIVATE ${SRC_OS}
    PRIVATE ${VULKAN_SDK}/include)

target_link_libraries(gx
    PUBLIC cx
    PUBLIC ui)
