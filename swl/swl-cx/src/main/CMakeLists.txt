cmake_minimum_required(VERSION 3.16.0)

set(HEADER_LIST
        public/core.hpp
        public/data.hpp
        public/file_manager.hpp
        public/game_loop.hpp
        public/header.hpp
        public/hidden_implem.hpp
        public/ownership.hpp
        public/point.hpp
        public/point_operators.hpp
        public/rect.hpp
        public/tell.hpp

        )

set(SOURCE_LIST
        cpp/core.cpp
        cpp/game_loop.cpp)

if(WIN32)
    set(PLATFORM_SOURCE_LIST
            platform/darwin/file_manager.mm)
endif()

if(APPLE)
    set(PLATFORM_SOURCE_LIST
            platform/darwin/file_manager.mm)
endif()

add_library(cx
    ${HEADER_LIST}
    ${SOURCE_LIST}
    ${PLATFORM_SOURCE_LIST}
)

target_include_directories(cx
    PUBLIC  public
    PRIVATE headers
    PRIVATE ${SRC_OS}
)
