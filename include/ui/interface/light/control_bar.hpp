#ifndef INCLUDE_UI_INTERFACE_LIGHT_CONTROL_BAR_HPP_NXI
#define INCLUDE_UI_INTERFACE_LIGHT_CONTROL_BAR_HPP_NXI

#include <ui/interface.hpp>

namespace ui
{
    class command_input;
    class user_session;
} // ui

namespace ui::interfaces::light
{
    class button;

    class control_bar : public ui::interface
    {
    public:
        control_bar(ui::user_session&, ui::window*);

        void enterEvent(QEnterEvent* event) override;
        void leaveEvent(QEvent* event) override;

        ui::command_input* command_input();

    private:
        void init_ui();

        ui::user_session& session_;

        ui::command_input* command_input_;
        button* command_root_;
        button* context_;
        button* page_root_;
    };
} // ui::interfaces::light

#endif // INCLUDE_UI_INTERFACE_LIGHT_CONTROL_BAR_HPP_NXI