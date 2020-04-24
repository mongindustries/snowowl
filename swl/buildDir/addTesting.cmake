
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
    add_subdirectory(swl-rd/src/test)

endif ()
