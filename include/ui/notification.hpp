#ifndef INCLUDE_UI_NOTIFICATION_HPP_NXI
#define INCLUDE_UI_NOTIFICATION_HPP_NXI

namespace nxi
{
    struct notification_data;
} // nxi

namespace ui
{
    class notification
    {
    public:
        virtual void show() = 0;

        static ui::notification* make(const nxi::notification_data& data, int notification_mode);
    };
} // ui

#endif // INCLUDE_UI_NOTIFICATION_HPP_NXI