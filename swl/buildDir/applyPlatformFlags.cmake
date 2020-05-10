function(apply_platform_flags)

    if (${CMAKE_BUILD_TYPE} MATCHES Debug)

        message("Debug build!")
        add_compile_definitions(SWL_DEBUG=1)

    endif (${CMAKE_BUILD_TYPE} MATCHES Debug)

    if (WIN32)

        if (WINDOWS_STORE)
            message("Apply platform macros and compile options for Windows Store")
            add_compile_definitions(SWL_UWP)
        else ()
            message("Apply platform macros and compile options for Windows")
        endif()

        set(SRC_OS "platform/windows")

        set(SWL_WIN32 PARENT_SCOPE)

        add_compile_options("/await")

        add_compile_definitions(SWL_WIN32=1)
        add_compile_definitions(UNICODE=1)
    endif (WIN32)

    if (APPLE)
        message("Apply platform macros and compile options for Darwin")
        set(SRC_OS "platform/darwin")

        set(SWL_DARWIN PARENT_SCOPE)

        add_compile_options(-fobjc-arc -fobjc-abi-version=2)
        add_compile_options(-stdlib=libc++)

        add_compile_definitions(SWL_DARWIN=1)
    endif (APPLE)

    message("Done!")

endfunction()
