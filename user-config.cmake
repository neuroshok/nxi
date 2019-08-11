# windows
if (NXI_TARGET_SYSTEM STREQUAL "windows")
    set(NXI_QT_ROOT C:/qt/5.13.0/msvc2017_64)

    # module
    module_build(strateon)
    module_build(test)
endif()

if (NXI_TARGET_SYSTEM STREQUAL "android")
    set(NXI_QT_ROOT C:/qt/5.13.0/android_arm64_v8a)
    set(NXI_ANDROID_ABI arm64-v8a)

    # android
    set(NXI_ANDROID_SDK_ROOT C:/android/android-sdk)
    set(NXI_ANDROID_NDK_ROOT C:/android/android-ndk-r18b)
    set(NXI_ANDROID_JDK_ROOT C:/java/jdk1.8.0_191)
endif()