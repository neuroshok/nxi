#ifndef INCLUDE_UI_INTERFACE_STANDARD_WINDOW_CONTROL_HPP_NXI
#define INCLUDE_UI_INTERFACE_STANDARD_WINDOW_CONTROL_HPP_NXI

#include <QPushButton>
#include <ui/interface.hpp>
#include <nxw/hbox_layout.hpp>
#include <nxw/icon_button.hpp>

namespace ui::interfaces::standard
{
    class window_control : public ui::interface
        {
        public:
            window_control(ui::user& user, ui::window* w)
                : interface(w)
            {
                /*
                auto layout = new nxw::hbox_layout;
                setLayout(layout);

                auto win_minimize = new nxw::icon_button(this, ":/icon/minimize");
                QObject::connect(win_minimize, &QPushButton::pressed, [this, &ui_core]()
                {
                    ui_core.window_system().minimize(window());
                });

                auto win_close = new nxw::icon_button(this, ":/icon/close");
                QObject::connect(win_close, &QPushButton::pressed, &ui_core, &ui::core::quit);

                layout->addWidget(win_minimize);
                layout->addWidget(new nxw::icon_button(this, ":/icon/maximize"));
                layout->addWidget(win_close);*/
            }
        };
} // ui

#endif // INCLUDE_UI_INTERFACE_STANDARD_WINDOW_CONTROL_HPP_NXI