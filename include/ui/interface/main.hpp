#ifndef INCLUDE_UI_INTERFACE_MAIN_HPP_NXI
#define INCLUDE_UI_INTERFACE_MAIN_HPP_NXI

#include <QWidget>

namespace ui
{
    class main_interface : public QWidget
    {
    public:
        struct style_type
        {
            QColor background_color = {238, 238, 238};
            QImage background_image;
        } style_data;

    public:
        main_interface()
            : fullmode_{ false }
            , style_data{}
        {}

        virtual void toggle_fullmode()
        {
            fullmode_ = !fullmode_;
        }

        bool fullmode() const
        {
            return fullmode_;
        }

    private:
        bool fullmode_;
    };
} // ui

#endif // INCLUDE_UI_INTERFACE_MAIN_HPP_NXI
