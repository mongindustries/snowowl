function(import_vulkan FILE)

    message("Reading ${FILE}...")

    file(READ ${FILE} PROPS)
    message("loca.prop: ${PROPS}")

    if(APPLE)
        string(REGEX MATCHALL [[vulkan\.sdk=((\/[a-zA-Z0-9._-]+)+)]] PROP_LIST ${PROPS})
    elseif(WIN32)
        string(REGEX MATCHALL [[vulkan\.sdk=([C-Z]:(\\[a-zA-Z0-9._-]+)+)]] PROP_LIST ${PROPS})
    endif()

    set(VULKAN_SDK ${CMAKE_MATCH_1} PARENT_SCOPE)
    message("Vulkan SDK location: ${VULKAN_SDK}")

endfunction()
