# windows
if (NXI_TARGET_SYSTEM STREQUAL "windows")
    set(QT_ROOT C:/qt/5.11.2/msvc2017_64)

    # module
    module_build(strateon)
    module_build(test)
endif()

if (NXI_TARGET_SYSTEM STREQUAL "android")
    set(QT_ROOT C:/qt/5.12.0/android_arm64_v8a)
    set(ANDROID_ABI arm64-v8a)
endif()

# android
set(ANDROID_SDK_ROOT C:/android/android-sdk)
set(ANDROID_NDK_ROOT C:/android/android-ndk-r18b)
set(ANDROID_JDK_ROOT C:/java/jdk1.8.0_191)