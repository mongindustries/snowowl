cmake_minimum_required(VERSION 3.16.0)

macro(package_add_test TESTNAME)

    file(GLOB HEADER_LIST
        "${VULKAN_SDK}/include"
        "../main/${PUB_HEAD}"
        "../main/${PRI_HEAD}"
        "../../../swl-cx/src/main/${PUB_HEAD}"
        "../../../swl-gx/src/main/${PUB_HEAD}")

    add_executable(${TESTNAME}
        ${ARGN})

    if(APPLE)
        target_link_libraries(${TESTNAME}
            gtest
            gmock
            cx
            gx
            rd
            c++
            "-framework AppKit"
            "-framework Cocoa"
            "-framework Foundation")
    endif()

    if(WIN32)
        target_link_libraries(${TESTNAME}
            gtest
            gmock
            cx
            gx
            rd)
    endif()

    target_include_directories(${TESTNAME}
        PUBLIC ${HEADER_LIST})

    add_test(NAME ${TESTNAME}s COMMAND ${TESTNAME})

endmacro()

package_add_test(rd-test
        cpp/main.cpp)
