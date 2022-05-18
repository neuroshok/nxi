#ifndef INCLUDE_UI_INTERFACE_MAIN_HPP_NXI
#define INCLUDE_UI_INTERFACE_MAIN_HPP_NXI

#include <QWidget>

namespace ui
{
    class window;

    class main_interface : public QWidget
    {
    public:
        struct style_type
        {
            QColor background_color = { 238, 238, 238 };
            QPixmap background_image;
        } style_data;

    public:
        main_interface(ui::window* window)
            : window_{ window }
            , fullmode_{ false }
            , style_data{}
        {}

        ui::window* window() { return window_; }

        virtual bool toggle_fullmode(int states = 2)
        {
            ++fullmode_;
            fullmode_ %= states; // 0 = normal, window = 1, screen = 2
            return fullmode_;
        }

        int fullmode() const
        {
            return fullmode_;
        }

    private:
        ui::window* window_;
        int fullmode_ = 0;
    };
} // ui

#endif // INCLUDE_UI_INTERFACE_MAIN_HPP_NXI