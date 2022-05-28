#ifndef INCLUDE_PLATFORM_WINDOWS_NOTIFICATION_HPP_NXI
#define INCLUDE_PLATFORM_WINDOWS_NOTIFICATION_HPP_NXI

#include <ui/notification.hpp>

#include <wintoast/wintoastlib.hpp>

#include <memory>

namespace nxi
{
    struct notification_data;
} // nxi

namespace WinToastLib
{
    class WinToastTemplate;
} // WinToastLib

namespace platform::windows
{
    class WinToastHandler;

    class notification : public ui::notification
    {
    public:
        explicit notification(const nxi::notification_data&);

        void show() override;

    private:
        std::unique_ptr<WinToastHandler> handler_;
        std::unique_ptr<WinToastLib::WinToastTemplate> template_;
    };
} // platform::windows

#endif // INCLUDE_PLATFORM_WINDOWS_NOTIFICATION_HPP_NXI