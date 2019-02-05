cmake_minimum_required(VERSION 3.0)

macro(nxi_android_apk_target BINARY_TARGET)

    # get toolchain info
    string(REGEX MATCH "${NXI_ANDROID_NDK_ROOT}/toolchains/(.*)-(.*)/prebuilt/.*" NXI_ANDROID_TOOLCHAIN_PARSED ${ANDROID_TOOLCHAIN_ROOT})
    if(NXI_ANDROID_TOOLCHAIN_PARSED)
        set(NXI_ANDROID_TOOLCHAIN_PREFIX ${CMAKE_MATCH_1})
        set(NXI_ANDROID_TOOLCHAIN_VERSION ${CMAKE_MATCH_2})
    else()
        message(FATAL_ERROR "Failed to parse NXI_ANDROID_TOOLCHAIN_ROOT to get toolchain prefix and version")
    endif()

    # get build-tools revision
    set(NXI_ANDROID_SDK_BUILDTOOLS_REVISION "0.0.0")
    file(GLOB ALL_BUILD_TOOLS_VERSIONS RELATIVE ${NXI_ANDROID_SDK_ROOT}/build-tools ${NXI_ANDROID_SDK_ROOT}/build-tools/*)
    foreach(BUILD_TOOLS_VERSION ${ALL_BUILD_TOOLS_VERSIONS})
        # find subfolder with greatest version
        if (${BUILD_TOOLS_VERSION} VERSION_GREATER ${NXI_ANDROID_SDK_BUILDTOOLS_REVISION})
            set(NXI_ANDROID_SDK_BUILDTOOLS_REVISION ${BUILD_TOOLS_VERSION})
        endif()
    endforeach()

    # config
    set(NXI_ANDROID_APP_NAME nxi)
    set(NXI_ANDROID_APP_PATH "${NXI_ANDROID_BUILD_ROOT}/lib${NXI_ANDROID_APP_NAME}.so")
    set(NXI_ANDROID_APP_VERSION_CODE 1)
    set(NXI_ANDROID_APP_VERSION "1.0")
    set(NXI_ANDROID_APP_PACKAGE_NAME org.neuroshok.nxi)
    set(NXI_ANDROID_APP_PACKAGE_SOURCE_ROOT "${NXI_ROOT}/platform/android/package")

    # STL
    set(NXI_ANDROID_STL_PATH "${ANDROID_STL_PATH}/libs/${NXI_ANDROID_ABI}/libc++_shared.so")

    # install option
    set(INSTALL_OPTIONS --reinstall)

    # todo
    # sign option
    if(ARG_KEYSTORE)
        set(SIGN_OPTIONS --release --sign ${ARG_KEYSTORE} --tsa http://timestamp.digicert.com)
        if(ARG_KEYSTORE_PASSWORD)
            set(SIGN_OPTIONS ${SIGN_OPTIONS} --storepass ${ARG_KEYSTORE_PASSWORD})
        endif()
    endif()

    # target level
    set(TARGET_LEVEL_OPTIONS --android-platform android-${NXI_ANDROID_PLATFORM_LEVEL})

    # make sure that the output directory for the Android package exists
    file(MAKE_DIRECTORY ${NXI_ANDROID_BUILD_ROOT}/libs/${NXI_ANDROID_ABI})

    # qt_deploy json
    configure_file(${NXI_ROOT}/platform/android/cmake/qt_deploy.json.in ${NXI_ANDROID_BUILD_ROOT}/qt_deploy.json @ONLY)
    # manifest
    configure_file(${NXI_ROOT}/platform/android/cmake/android_manifest.xml.in ${NXI_ANDROID_APP_PACKAGE_SOURCE_ROOT}/AndroidManifest.xml @ONLY)

    # apk target
    add_custom_target(${BINARY_TARGET}_apk
        ALL
        DEPENDS ${BINARY_TARGET}
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${NXI_ANDROID_BUILD_ROOT}/libs/${NXI_ANDROID_ABI} # it seems that recompiled libraries are not copied if we don't remove them first
        COMMAND ${CMAKE_COMMAND} -E make_directory ${NXI_ANDROID_BUILD_ROOT}/libs/${NXI_ANDROID_ABI}
        COMMAND ${CMAKE_COMMAND} -E copy ${NXI_ANDROID_APP_PATH} ${NXI_ANDROID_BUILD_ROOT}/libs/${NXI_ANDROID_ABI}
        COMMAND ${NXI_QT_ROOT}/bin/androiddeployqt
            --verbose
            --output ${NXI_ANDROID_BUILD_ROOT}
            --input ${NXI_ANDROID_BUILD_ROOT}/qt_deploy.json
            --gradle ${TARGET_LEVEL_OPTIONS} ${INSTALL_OPTIONS} ${SIGN_OPTIONS}
        COMMAND ${NXI_ANDROID_SDK_ROOT}/platform-tools/adb.exe shell am start -S -n ${NXI_ANDROID_APP_PACKAGE_NAME}/org.qtproject.qt5.android.bindings.QtActivity
    )
endmacro()
