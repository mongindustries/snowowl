cmake_minimum_required(VERSION 3.16.0)

macro(package_add_test TESTNAME)

    file(GLOB HEADER_LIST
            "../main/${PUB_HEAD}"
            "../main/${PRI_HEAD}"
            "../main/${PRI_HEAD}"
            "../../../swl-cx/src/main/${PUB_HEAD}")

    add_executable(${TESTNAME}
            ${ARGN})

    target_link_libraries(${TESTNAME}
            gtest
            gmock
            cx
            ui
            "-framework AppKit"
            "-framework Cocoa"
            "-framework Foundation")

    target_include_directories(${TESTNAME}
            PUBLIC ${HEADER_LIST})

    add_test(NAME ${TESTNAME}s COMMAND ${TESTNAME})

endmacro()

if (WINDOWS)
    package_add_test(ui-test
            cpp/main_win32.cpp)
endif()

if (APPLE)
    package_add_test(ui-test
            cpp/main_darwin.cpp)
endif()
