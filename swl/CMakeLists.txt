cmake_minimum_required(VERSION 3.16.0)

if(APPLE)
project(snow_owl VERSION 0.1.0 LANGUAGES CXX OBJC)
endif()

if(WIN32)
project(snow_owl VERSION 0.1.0 LANGUAGES CXX)
endif()

# c++ stuff

set(CMAKE_CXX_STANDARD          20  )
set(CMAKE_CXX_STANDARD_REQUIRED ON  )
set(CMAKE_CXX_EXTENSIONS        OFF )

# variables

set(SRC_ROOT "cpp")

set(PRI_HEAD "headers")
set(PUB_HEAD "public")

file(READ local.properties PROPS)
message("loca.prop: ${PROPS}")

if(APPLE)
    string(REGEX MATCHALL [[vulkan\.sdk=((\/[a-zA-Z0-9._-]+)+)]] PROP_LIST ${PROPS})
elseif(WIN32)
    string(REGEX MATCHALL [[vulkan\.sdk=([C-Z]:(\\[a-zA-Z0-9._-]+)+)]] PROP_LIST ${PROPS})
endif()

set(VULKAN_SDK ${CMAKE_MATCH_1})
message("Vulkan SDK location: ${VULKAN_SDK}")

option(PACKAGE_TESTS "Build tests" ON)

# platform files and macros

if (${CMAKE_BUILD_TYPE} MATCHES Debug)
    message("Debug build!")
    add_compile_definitions(SWL_DEBUG=1)
endif (${CMAKE_BUILD_TYPE} MATCHES Debug)

if (WIN32)
    message("Apply platform and macros for Windows...")
    set(SRC_OS "platform/windows")
    add_compile_definitions(SWL_WIN32=1)
endif (WIN32)

if (APPLE)
    message("Apply platform and macros for Darwin...")
    set(SRC_OS "platform/darwin")
    add_compile_options(-fobjc-arc)

    add_compile_definitions(SWL_DARWIN=1)
endif (APPLE)

add_subdirectory(swl-cx/src/main)
add_subdirectory(swl-ui/src/main)
add_subdirectory(swl-gx/src/main)
add_subdirectory(app/src/main)

if (PACKAGE_TESTS)

    enable_testing()
    include(GoogleTest)

    add_subdirectory("${PROJECT_SOURCE_DIR}/deps/googletest/lib" "deps/googletest")

    mark_as_advanced(
            BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
            gmock_build_tests gtest_build_samples gtest_build_tests
            gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols)

    set_target_properties(gtest PROPERTIES FOLDER extern)
    set_target_properties(gtest_main PROPERTIES FOLDER extern)
    set_target_properties(gmock PROPERTIES FOLDER extern)
    set_target_properties(gmock_main PROPERTIES FOLDER extern)

    add_subdirectory(swl-cx/src/test)
    add_subdirectory(swl-gx/src/test)
    add_subdirectory(swl-ui/src/test)

endif ()
