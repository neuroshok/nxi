#include <ui/notification.hpp>

#include <nxi/notification_data.hpp>

#include <platform/generic/notification.hpp>
#include <platform/notification.hpp>

namespace ui
{
    ui::notification* notification::make(const nxi::notification_data& data, int notification_mode)
    {
        if (notification_mode == 0)
        {
            return new platform::generic::notification{ data };
        }
        else
        {
            return new platform::notification{ data };
        }
    }
} // ui