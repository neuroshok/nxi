
set(ANDROID_GENERATOR "MinGW Makefiles")
set(ANDROID_MAKE_PROGRAM ${ANDROID_NDK_ROOT}/prebuilt/windows-x86_64/bin/make.exe)
set(ANDROID_TOOLCHAIN_FILE ${ANDROID_NDK_ROOT}/build/cmake/android.toolchain.cmake)

# android build target
# create build output for android
if(NOT EXISTS "${CMAKE_BINARY_DIR}/android_build")
    file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/android_build")
endif()

# SDK defined, create a target for android
if (NXI_TARGET_SYSTEM STREQUAL "android")
    set(CMAKE_GENERATOR "MinGW Makefiles" CACHE INTERNAL "" FORCE)
    set(CMAKE_MAKE_PROGRAM ${ANDROID_MAKE_PROGRAM} CACHE INTERNAL "" FORCE)
    set(CMAKE_TOOLCHAIN_FILE ${ANDROID_TOOLCHAIN_FILE} CACHE INTERNAL "" FORCE)

    set(QT_COMPONENTS Core Widgets Gui WebView Qml Quick QuickWidgets 3DCore 3DRender 3DInput 3DExtras)
    set(QT_LIBS Qt5::Core Qt5::Widgets Qt5::Gui Qt5::WebView Qt5::Qml Qt5::Quick Qt5::QuickWidgets Qt5::3DCore Qt5::3DRender Qt5::3DInput Qt5::3DExtras)

    # sources
    #file(GLOB_RECURSE NXI_SOURCES "include/nxw/*.hpp")

    set(NXI_SOURCES ${NXI_SOURCES}
            source/platform/android/main.cpp
            source/platform/android/qml.qrc
            source/ui/image.qrc
            include/nxw/hbox_layout.hpp
            include/nxw/icon_button.hpp)

endif()

# command to build apk
add_custom_target(
        nxi_android_deploy
        ALL
        COMMAND ${CMAKE_COMMAND} -G \"${ANDROID_GENERATOR}\"
        -DNXI_TARGET_SYSTEM=android
        -DCMAKE_MAKE_PROGRAM=${ANDROID_MAKE_PROGRAM}
        -DCMAKE_TOOLCHAIN_FILE=${ANDROID_TOOLCHAIN_FILE}
        ../..
        COMMAND ${CMAKE_COMMAND} --build . --target nxi_android_apk
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/android_build"
)