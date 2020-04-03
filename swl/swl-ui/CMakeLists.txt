cmake_minimum_required(VERSION 3.16.0)

file(GLOB HEADER_LIST "${PUB_HEAD}/*.hpp" "${PRI_HEAD}/*.hxx" "${PRI_HEAD}/*.hpp")
file(GLOB SOURCE_LIST "${SRC_ROOT}/*.cpp")

file(GLOB PLATFORM_HEAD "${SRC_OS}/*.hxx" "${SRC_OS}/*.hpp")
file(GLOB PLATFORM_SRC "${SRC_OS}/*.cxx" "${SRC_OS}/*.cpp")

file(GLOB CX_HEADERS "../swl-cx/${PUB_HEAD}")

add_library(ui
    ${HEADER_LIST}
    ${SOURCE_LIST}
    ${PLATFORM_HEAD}
    ${PLATFORM_SRC}
)

include_directories(
    ${PUB_HEAD}
    ${PRI_HEAD}
    ${PLATFOMR_HEAD}
    ${CX_HEADERS}
)

target_include_directories(ui
    INTERFACE ${CX_HEADERS}
)

target_link_libraries(ui
    PUBLIC cx
)
