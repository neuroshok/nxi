// clang-format off

#ifndef INCLUDE_PLATFORM_NOTIFICATION_HPP_NXI
#define INCLUDE_PLATFORM_NOTIFICATION_HPP_NXI

#include <QtGlobal>

#ifdef NXI_NOTIFICATION_PLATFORM
    #if defined(Q_OS_WIN)
        #include <platform/windows/notification.hpp>
        namespace platform
        {
            using namespace platform::windows;
        }
    #elif defined(Q_OS_MACOS)
        #include <platform/macos/notification.hpp>
        namespace platform
        {
            using namespace platform::macos;
        }
    #else
        #error "Unsupported platform"
    #endif
#else // generic notification
    #include <platform/generic/notification.hpp>
    namespace platform
    {
        using namespace platform::generic;
    }
#endif

#endif // INCLUDE_PLATFORM_NOTIFICATION_HPP_NXI