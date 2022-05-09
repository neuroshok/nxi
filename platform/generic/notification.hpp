#ifndef PLATFORM_GENERIC_NOTIFICATION_HPP_NXI
#define PLATFORM_GENERIC_NOTIFICATION_HPP_NXI

#include <ui/notification.hpp>

namespace nxi
{
    struct notification_data;
} // nxi

namespace platform::generic
{
    class notification : public ui::notification
    {
    public:
        explicit notification(const nxi::notification_data& data);

        void show() override;

    private:
    };
} // platform::generic

#endif // PLATFORM_GENERIC_NOTIFICATION_HPP_NXI