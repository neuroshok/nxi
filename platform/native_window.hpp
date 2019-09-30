#ifndef PLATFORM_NATIVE_WINDOW_HPP_NXI
#define PLATFORM_NATIVE_WINDOW_HPP_NXI

#include <QtGlobal>

#ifdef Q_OS_WIN
    #include <platform/windows/native_window.hpp>
    namespace platform
    {
        using namespace platform::windows;
    }
#else
    #include <platform/generic/native_window.hpp>
#endif

#endif // PLATFORM_NATIVE_WINDOW_HPP_NXI
