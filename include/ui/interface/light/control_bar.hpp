#ifndef INCLUDE_UI_INTERFACE_LIGHT_CONTROL_BAR_HPP_NXI
#define INCLUDE_UI_INTERFACE_LIGHT_CONTROL_BAR_HPP_NXI

#include <ui/interface.hpp>

namespace ui
{
    class core;
    class command;
} // ui

namespace ui::interfaces::light
{
    class button;

    class control_bar : public ui::interface
    {
    public:
        control_bar(ui::core& ui_core, ui::window* window);

        void enterEvent(QEvent* event) override;
        void leaveEvent(QEvent* event) override;

        ui::command* command_input();

    private:
        ui::core& ui_core_;

        ui::command* command_input_;
        button* command_root_;
        button* context_;
        button* page_root_;
    };
} // ui::interfaces::light

#endif // INCLUDE_UI_INTERFACE_LIGHT_CONTROL_BAR_HPP_NXI