cmake_minimum_required(VERSION 3.16.0)

file(GLOB HEADER_LIST "${PUB_HEAD}/*.hpp" "${PRI_HEAD}/*.hxx" "${PRI_HEAD}/*.hpp")
file(GLOB SOURCE_LIST "${SRC_ROOT}/*.cpp")

file(GLOB PLATFORM_HEAD "${SRC_OS}/*.hxx" "${SRC_OS}/*.hpp")
file(GLOB PLATFORM_SRC "${SRC_OS}/*.cxx" "${SRC_OS}/*.cpp")

add_library(gx
    ${HEADER_LIST}
    ${SOURCE_LIST}
    ${PLATFORM_HEAD}
    ${PLATFORM_SRC}
)

include_directories(
    ${PRI_HEAD}
    ${PUB_HEAD}
    ${PLATFORM_HEAD}
)

file(GLOB CX_HEADERS "../swl-cx/${PUB_HEAD}")
file(GLOB UI_HEADERS "../swl-ui/${PUB_HEAD}")

target_include_directories(gx
    INTERFACE ${CX_HEADERS}
    INTERFACE ${UI_HEADERS}
)

target_link_libraries(gx
    PUBLIC cx
    PUBLIC ui
)
