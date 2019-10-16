#ifndef INCLUDE_UI_INTERFACE_LIGHT_CONTROL_BAR_HPP_NXI
#define INCLUDE_UI_INTERFACE_LIGHT_CONTROL_BAR_HPP_NXI

#include <ui/interface.hpp>

namespace ui
{
    class core;
    class command;
    class command_menu;
} // ui

namespace ui::interfaces::light
{
    class control_bar : public ui::interface
    {
    public:

        explicit control_bar(ui::core& ui_core, ui::window* window);

    private:
        ui::core& ui_core_;

        ui::command* command_input_;
    };
} // ui::interfaces::light

#endif // INCLUDE_UI_INTERFACE_LIGHT_CONTROL_BAR_HPP_NXI