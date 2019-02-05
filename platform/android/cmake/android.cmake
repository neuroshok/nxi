
set(NXI_ANDROID_GENERATOR "MinGW Makefiles")
set(NXI_ANDROID_MAKE_PROGRAM ${NXI_ANDROID_NDK_ROOT}/prebuilt/windows-x86_64/bin/make.exe)
set(NXI_ANDROID_TOOLCHAIN_FILE ${NXI_ANDROID_NDK_ROOT}/build/cmake/android.toolchain.cmake)

set(NXI_ANDROID_BUILD_ROOT ${NXI_ROOT}/cmake-build-android-${CMAKE_BUILD_TYPE})

# create build output for android
file(MAKE_DIRECTORY "${NXI_ANDROID_BUILD_ROOT}")

if (NXI_TARGET_SYSTEM STREQUAL "android")

    # check the JAVA_HOME environment variable
    # (I couldn't find a way to set it from this script, it has to be defined outside)
    set(JAVA_HOME $ENV{JAVA_HOME})
    if(NOT JAVA_HOME)
        message(FATAL_ERROR "The JAVA_HOME environment variable is not set. Please set it to the root directory of the JDK.")
    endif()

    # api level
    if(NOT NXI_ANDROID_PLATFORM_LEVEL)
        set(NXI_ANDROID_PLATFORM_LEVEL 28)
    endif()
    set(ANDROID_NDK_PLATFORM android-${NXI_ANDROID_PLATFORM_LEVEL})
    set(ANDROID_PLATFORM ${ANDROID_NDK_PLATFORM})
    set(ANDROID_ABI ${NXI_ANDROID_ABI})

    # toolchain
    set(CMAKE_GENERATOR "MinGW Makefiles" CACHE INTERNAL "" FORCE)
    set(CMAKE_MAKE_PROGRAM ${NXI_ANDROID_MAKE_PROGRAM} CACHE INTERNAL "" FORCE)
    set(CMAKE_TOOLCHAIN_FILE ${NXI_ANDROID_TOOLCHAIN_FILE} CACHE INTERNAL "" FORCE)

    # qt
    set(NXI_QT_COMPONENTS Core Widgets Gui WebView Qml Quick QuickControls2 QuickWidgets 3DCore 3DRender 3DInput 3DExtras)
    set(NXI_QT_LIBS Qt5::Core Qt5::Widgets Qt5::Gui Qt5::WebView Qt5::Qml Qt5::Quick Qt5::QuickWidgets Qt5::QuickControls2 Qt5::3DCore Qt5::3DRender Qt5::3DInput Qt5::3DExtras)

    # sources
    set(NXI_SOURCES ${NXI_SOURCES}
            platform/android/source/main.cpp
            platform/android/source/qml.qrc
            source/ui/image.qrc
            include/nxw/hbox_layout.hpp
            include/nxw/icon_button.hpp)

endif()

# command to build apk
add_custom_target(
        nxi_android_deploy
        ALL
        COMMAND ${CMAKE_COMMAND} -G \"${NXI_ANDROID_GENERATOR}\"
        -DNXI_TARGET_SYSTEM=android
        -DNXI_ANDROID_ABI=${ANDROID_ABI}
        -DCMAKE_MAKE_PROGRAM=${NXI_ANDROID_MAKE_PROGRAM}
        -DCMAKE_TOOLCHAIN_FILE=${NXI_ANDROID_TOOLCHAIN_FILE}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        ..
        COMMAND ${CMAKE_COMMAND} --build . --target nxi_apk
        WORKING_DIRECTORY "${NXI_ANDROID_BUILD_ROOT}"
)

include(${NXI_ROOT}/platform/android/cmake/android_apk.cmake)