function(apply_platform_flags)

    if (${CMAKE_BUILD_TYPE} MATCHES Debug)

        message("Debug build!")
        add_compile_definitions(SWL_DEBUG=1)

    endif (${CMAKE_BUILD_TYPE} MATCHES Debug)

    if (WIN32)
        message("Apply platform macros and compile options for Windows")
        set(SRC_OS "platform/windows")

        add_compile_options(/EHs)
        add_compile_definitions(SWL_WIN32=1)
        add_compile_definitions(UNICODE=1)
    endif (WIN32)

    if (APPLE)
        message("Apply platform macros and compile options for Darwin")
        set(SRC_OS "platform/darwin")

        add_compile_options(-fobjc-arc)
        add_compile_definitions(SWL_DARWIN=1)
    endif (APPLE)

    message("Done!")

endfunction()
