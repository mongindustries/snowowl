cmake_minimum_required(VERSION 3.16.0)

set(HEADER_LIST
        public/core.hpp
        public/data.hpp
        public/headerconv.hpp
        public/hidden_implem.hpp
        public/ownership.hpp
        public/path.hpp
        public/point.hpp
        public/point_operators.hpp
        public/rect.hpp
        public/tell.hpp

        headers/path_ops.hpp)

set(SOURCE_LIST
        cpp/core.cpp
        cpp/path.cpp
        cpp/path_ops.cpp
        cpp/point_operators.cpp)

if(WINDOWS)
    set(PLATFORM_SOURCE_LIST
            platform/windows/PathImpl.cpp)
endif()

if(APPLE)
    set(PLATFORM_SOURCE_LIST
            platform/darwin/PathImpl.cpp)
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
