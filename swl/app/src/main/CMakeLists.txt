cmake_minimum_required(VERSION 3.16.0)

add_executable(app
    cpp/app.cpp
        )

target_include_directories(app
    PUBLIC ../../../swl-cx/src/main/public
    PUBLIC ../../../swl-ui/src/main/public
    PUBLIC ../../../swl-gx/src/main/public)

target_link_libraries(app
    cx
    ui
    gx)

if(APPLE)

    target_link_libraries(app
        "-framework Foundation"
        "-framework QuartzCore"
        "-framework AppKit"
        "-framework Metal")

endif()
