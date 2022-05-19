#ifndef INCLUDE_UI_INTERFACE_MAIN_HPP_NXI
#define INCLUDE_UI_INTERFACE_MAIN_HPP_NXI

#include <ui/interface.hpp>
#include <QWidget>

namespace ui
{
    class window;

    class main_interface : public ui::interface
    {
    public:
        struct style_type
        {
            QColor background_color = { 238, 238, 238 };
            QPixmap background_image;
        } style_data;

    public:
        explicit main_interface(ui::window*);

        virtual bool toggle_fullmode(int states = 2);

        [[nodiscard]] int fullmode() const;

    private:
        int fullmode_;
    };
} // ui

#endif // INCLUDE_UI_INTERFACE_MAIN_HPP_NXI