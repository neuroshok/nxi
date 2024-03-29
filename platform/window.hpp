#ifndef PLATFORM_WINDOW_HPP_NXI
#define PLATFORM_WINDOW_HPP_NXI

#include <QtGlobal>

#if defined(Q_OS_WIN) && !defined(NXI_WINDOW_GENERIC)
    #include <platform/windows/window.hpp>
    namespace platform { using namespace platform::windows; }
#else
    #include <platform/generic/window.hpp>
    namespace platform { using namespace platform::generic; }
#endif

#endif // PLATFORM_WINDOW_HPP_NXI
