cmake_minimum_required(VERSION 3.16.0)

macro(package_add_test TESTNAME)

    file(GLOB HEADER_LIST
        "../main/${PUB_HEAD}"
        "../main/${PRI_HEAD}"
        "../main/${PRI_HEAD}"
        "../../../swl-cx/src/main/${PUB_HEAD}"
        "../../../swl-ui/src/main/${PUB_HEAD}")

    add_executable(${TESTNAME}
        ${ARGN})

    target_link_libraries(${TESTNAME}
        gtest
        gmock
        cx
        gx
        ui
        "-framework AppKit"
        "-framework Cocoa"
        "-framework Foundation")

    target_include_directories(${TESTNAME}
        PUBLIC ${HEADER_LIST})

    add_test(NAME ${TESTNAME}s COMMAND ${TESTNAME})

endmacro()

package_add_test(gx-test
    cpp/context_test.cpp
    cpp/main.cpp)
