function(compile_shader TARGET FILE TYPE)

    set(input_path  ${CMAKE_CURRENT_SOURCE_DIR}/hlsl/${FILE}.${TYPE})
    set(output_path ${CMAKE_CURRENT_SOURCE_DIR}/spirv/${FILE}-${TYPE}.spv)

    get_filename_component(output_dir ${output_path} DIRECTORY)
    file(MAKE_DIRECTORY ${output_dir})

    get_filename_component(input_dir ${input_path} DIRECTORY)

    source_group(TREE ${input_dir}  PREFIX shader FILES ${input_path})
    source_group(TREE ${output_dir} PREFIX shader-binary FILES ${output_path})

    if(APPLE)

        file(GLOB vulkan-tools ${vulkan_sdk}/macOS/bin/*)
        find_program(GV glslc HINTS ${vulkan-tools})

        if(${CMAKE_BUILD_TYPE} MATCHES Debug)
            set(COMMAND ${GV} --target-spv=spv1.0 -fentry-point=main -o ${output_path} -x hlsl -O0 -g ${input_path})
        else()
            set(COMMAND ${GV} --target-spv=spv1.0 -fentry-point=main -o ${output_path} -x hlsl ${input_path})
        endif()

        add_custom_command(
            OUTPUT              ${output_path}
            COMMAND             ${COMMAND}
            DEPENDS             ${input_path}
            IMPLICIT_DEPENDS    CXX ${input_path}
            COMMENT             ${COMMAND}
            VERBATIM)

        set_source_files_properties(${output_path} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

    endif(APPLE)

    if(WIN32)
        
        file(GLOB dxc ${CMAKE_SOURCE_DIR}/../dxc/*.exe)
        find_program(DXC dxc.exe HINTS ${dxc})

        if(${TYPE} STREQUAL "frag")
            set(type "ps_6_0")
        endif(${TYPE} STREQUAL "frag")
        
        if(${TYPE} STREQUAL "vert")
            set(type "vs_6_0")
        endif(${TYPE} STREQUAL "vert")

        file(TO_NATIVE_PATH "${input_dir}" input_dir_win)

        if(${CMAKE_BUILD_TYPE} MATCHES Debug)
            set(COMMAND ${DXC} -I ${input_dir_win} -H -T ${type} -E main -Od -Zi -Fo ${output_path} ${input_path})
        else()
            set(COMMAND ${DXC} -I ${input_dir_win} -T ${type} -E main -Fo ${output_path} ${input_path})
        endif()

        add_custom_command(
            OUTPUT              ${output_path}
            COMMAND             ${COMMAND}
            DEPENDS             ${input_path}
            IMPLICIT_DEPENDS    CXX ${input_path}
            COMMENT             "dxc compile"
            VERBATIM)

        set_target_properties(${TARGET} PROPERTIES RESOURCE ${output_path})

    endif(WIN32)

    set_source_files_properties(${output_path} PROPERTIES GENERATED TRUE)
    target_sources(${TARGET} PRIVATE ${output_path})

endfunction()

function(get_shader_files TARGET)

    file(GLOB frag-files ${CMAKE_CURRENT_SOURCE_DIR}/hlsl/*.frag)
    file(GLOB vert-files ${CMAKE_CURRENT_SOURCE_DIR}/hlsl/*.vert)

    foreach(file ${vert-files})

        get_filename_component(p ${file} NAME_WE)
        compile_shader(${TARGET} ${p} vert)

    endforeach(file ${vert-files})

    foreach(file ${frag-files})

    get_filename_component(p ${file} NAME_WE)
        compile_shader(${TARGET} ${p} frag)

    endforeach(file ${frag-files})

endfunction(get_shader_files)
