function(compile_shader TARGET FILE TYPE)

    set(input_path  ${CMAKE_CURRENT_SOURCE_DIR}/hlsl/${FILE}.${TYPE})
    set(output_path ${CMAKE_BINARY_DIR}/${TARGET}/src/main/${FILE}-${TYPE}.shader)

    get_filename_component(output_dir ${output_path} DIRECTORY)
    file(MAKE_DIRECTORY ${output_dir})

    get_filename_component(input_dir ${input_path} DIRECTORY)

    source_group(TREE ${input_dir}  PREFIX shader FILES ${input_path})
    source_group(TREE ${output_dir} PREFIX shader-binary FILES ${output_path})

    if(APPLE)

        file(GLOB vulkan-tools ${vulkan_sdk}/macOS/bin/*)
        find_program(GV glslc HINTS ${vulkan-tools})

        set_source_files_properties(${output_path} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

        if(${CMAKE_BUILD_TYPE} MATCHES Debug)
            message(${GV} --target-spv=spv1.0 -fentry-point=main -o ${output_path} -x hlsl -O0 -g ${input_path})
            set(COMMAND ${GV} --target-spv=spv1.0 -fentry-point=main -o ${output_path} -x hlsl -O0 -g ${input_path})
        else()
            message(${GV} --target-spv=spv1.0 -fentry-point=main -o ${output_path} -x hlsl ${input_path})
            set(COMMAND ${GV} --target-spv=spv1.0 -fentry-point=main -o ${output_path} -x hlsl ${input_path})
        endif()

    endif(APPLE)

    if(WIN32)

        file(GLOB vs-tools "C:/Program Files (x86)/Windows Kits/10/bin/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/x64/*")
        find_program(GV dxc HINTS ${vs-tools})
        
        set_target_properties(${TARGET} PROPERTIES RESOURCE ${output_path})

        if ("${TYPE}" STREQUAL "frag")
            set(OTYPE ps_6_0)
        elseif ("${TYPE}" STREQUAL "vert")
            set(OTYPE vs_6_0)
        endif ()

        file(TO_NATIVE_PATH ${output_path} win_output_path)
        file(TO_NATIVE_PATH ${input_path} win_input_path)
        file(TO_NATIVE_PATH ${input_dir} win_input_dir)

        file(TO_NATIVE_PATH ${GV} dxc)

        if(${CMAKE_BUILD_TYPE} MATCHES Debug)
            set(COMMAND "${dxc}" -T ${OTYPE} -E main -I ${win_input_dir} -Fh ${win_output_path} -Vn ${TYPE}_prog -Zi -O0 ${win_input_path})
        else()
            set(COMMAND "${dxc}" -T ${OTYPE} -E main -I ${win_input_dir} -Fh ${win_output_path} -Vn ${TYPE}_prog ${win_input_path})
        endif()

    endif(WIN32)

    add_custom_command(
        OUTPUT              ${output_path}
        COMMAND             ${COMMAND}
        DEPENDS             ${input_path}
        IMPLICIT_DEPENDS    CXX ${input_path}
        COMMENT             Compile ${TYPE} shader ${input_path}
        VERBATIM)

    set_source_files_properties(${output_path} PROPERTIES GENERATED TRUE)
    target_sources(${TARGET} PRIVATE ${output_path})

    target_include_directories(${TARGET} PRIVATE ${output_dir})
    
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
