cmake_minimum_required(VERSION 3.16.0)

macro(package_add_test TESTNAME)

    file(GLOB HEADER_LIST
        "../main/${PUB_HEAD}"
        "../main/${PRI_HEAD}"
        "../main/${PRI_HEAD}")

    add_executable(${TESTNAME}
        ${ARGN})

    target_link_libraries(${TESTNAME}
        gtest
        gmock
        cx)
    target_include_directories(${TESTNAME}
        PUBLIC ${HEADER_LIST})

    add_test(NAME ${TESTNAME}s COMMAND ${TESTNAME})

endmacro()

package_add_test(cx-test
    cpp/ownership_test.cpp
    cpp/path_test.cpp
    cpp/point_test.cpp
    cpp/main.cpp
)
