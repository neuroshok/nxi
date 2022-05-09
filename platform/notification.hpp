#ifndef PLATFORM_NOTIFICATION_HPP_NXI
#define PLATFORM_NOTIFICATION_HPP_NXI

#include <QtGlobal>

#if defined(Q_OS_WIN) && !defined(NXI_NOTIFICATION_GENERIC)
#include <platform/windows/notification.hpp>
namespace platform
{
    using namespace platform::windows;
}
#elif defined(Q_OS_MACOS) && !defined(NXI_NOTIFICATION_GENERIC)
#include <platform/macos/notification.hpp>
namespace platform
{
    using namespace platform::macos;
}
#else
#include <platform/generic/notification.hpp>
namespace platform
{
    using namespace platform::generic;
}
#endif

#endif // PLATFORM_NOTIFICATION_HPP_NXI