cmake_minimum_required(VERSION 3.16.0)

set(VERTEX_SOURCES
    hlsl/common.h
    hlsl/simple.vert)

set(FRAGMENT_SOURCES
    hlsl/common.h
    hlsl/simple.frag)

set(APP_SOURCES
    ${FRAGMENT_SOURCES}
    ${VERTEX_SOURCES}
    cpp/app.cpp)

if(WIN32)

  if(WINDOWS_STORE)

    set(logos
      platform/uwp/SmallLogo44x44.scale-100.png
      platform/uwp/SmallLogo44x44.scale-200.png
      platform/uwp/SmallLogo44x44.scale-400.png
      platform/uwp/SmallLogo44x44.altform-lightunplated_targetsize-16.png
      platform/uwp/SmallLogo44x44.altform-lightunplated_targetsize-24.png
      platform/uwp/SmallLogo44x44.altform-lightunplated_targetsize-32.png
      platform/uwp/SmallLogo44x44.altform-lightunplated_targetsize-48.png
      platform/uwp/SmallLogo44x44.altform-lightunplated_targetsize-256.png
      platform/uwp/SmallLogo44x44.altform-unplated_targetsize-16.png
      platform/uwp/SmallLogo44x44.altform-unplated_targetsize-24.png
      platform/uwp/SmallLogo44x44.altform-unplated_targetsize-32.png
      platform/uwp/SmallLogo44x44.altform-unplated_targetsize-48.png
      platform/uwp/SmallLogo44x44.altform-unplated_targetsize-256.png
      platform/uwp/SmallLogo44x44.targetsize-16.png
      platform/uwp/SmallLogo44x44.targetsize-24.png
      platform/uwp/SmallLogo44x44.targetsize-32.png
      platform/uwp/SmallLogo44x44.targetsize-48.png
      platform/uwp/SmallLogo44x44.targetsize-256.png
      platform/uwp/Logo.scale-100.png
      platform/uwp/Logo.scale-200.png
      platform/uwp/Logo.scale-400.png
      platform/uwp/SplashScreen.scale-100.png
      platform/uwp/SplashScreen.scale-200.png
      platform/uwp/SplashScreen.scale-400.png
      platform/uwp/StoreLogo.scale-100.png
      platform/uwp/StoreLogo.scale-200.png
      platform/uwp/StoreLogo.scale-400.png)

    set(manifest
      platform/uwp/package.appxManifest)

    set(uwp_content
      ${logos} ${manifest}
      platform/uwp/app_TemporaryKey.pfx)

    set_property(SOURCE ${manifest} PROPERTY VS_DEPLOYMENT_CONTENT 1)
    set_property(SOURCE ${logos} PROPERTY VS_DEPLOYMENT_CONTENT 1)
    set_property(SOURCE ${logos} PROPERTY VS_DEPLOYMENT_LOCATION "Assets")

    add_executable(app WIN32
            ${APP_SOURCES}
            ${uwp_content})

    target_link_libraries(app
        "dwmapi"
        "dxgi"
        "d3d12")
    
    set_target_properties(app PROPERTIES VS_CPPWINRT true)

  else()

    add_executable(app WIN32
            ${APP_SOURCES}
            platform/windows/app.rc
            platform/windows/resource.h)

    target_link_libraries(app
        "dwmapi"
        "dxgi"
        "d3d12"
        "dcomp"
        "windowsapp")

  endif()

  get_shader_files(app)

endif()

if(APPLE)
    add_executable(app MACOSX_BUNDLE
        ${APP_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/platform/darwin/app.xcassets)

    set_target_properties(app PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/platform/darwin/Info.plist.in
        XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME AppIcon)

    set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/platform/darwin/app.xcassets
        PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

    target_link_libraries(app
            c++
            "-framework Foundation"
            "-framework QuartzCore"
            "-framework AppKit"
            "-framework Metal"
            "-framework IOKit"
            "-framework IOSurface")
endif()

target_include_directories(app
    PUBLIC headers
    PUBLIC ./
    PUBLIC ../../../swl-cx/src/main/public
    PUBLIC ../../../swl-ui/src/main/public
    PUBLIC ../../../swl-gx/src/main/public
    PUBLIC ${VULKAN_SDK}/include)

target_link_libraries(app
    cx
    ui
    gx)
