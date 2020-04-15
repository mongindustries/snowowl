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
            set(COMMAND ${GV} --target-spv=spv1.0 -fentry-point=main -o ${output_path} -x hlsl ${input_path} -O0 -g)
        else()
            set(COMMAND ${GV} --target-spv=spv1.0 -fentry-point=main -o ${output_path} -x hlsl ${input_path})
        endif()

        add_custom_command(
            OUTPUT              ${output_path}
            COMMAND             ${COMMAND}
            DEPENDS             ${input_path}
            IMPLICIT_DEPENDS    CXX ${input_path}
            COMMENT             "glslangValidator -e main -o ${output_path} -V -D ${input_path}"
            VERBATIM)

        set_source_files_properties(${output_path} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

    endif(APPLE)

    if(WIN32)
        
        file(GLOB dxc ${CMAKE_SOURCE_DIR}/../dxc/*.exe)
        find_program(DXC dxc.exe HINTS ${dxc})

        if(${CMAKE_BUILD_TYPE} MATCHES Debug)
            set(COMMAND ${DXC} -E main -Od -Zi)
        else()
            set(COMMAND ${DXC} -E main)
        endif()

        add_custom_command(
            OUTPUT              ${output_path}
            COMMAND             ${COMMAND}
            DEPENDS             ${input_path}
            IMPLICIT_DEPENDS    CXX ${input_path}
            COMMENT             ""
            VERBATIM)

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
