#ifndef INCLUDE_UI_INTERFACE_LIGHT_ACCESS_BAR_HPP_NXI
#define INCLUDE_UI_INTERFACE_LIGHT_ACCESS_BAR_HPP_NXI

#include <ui/element.hpp>

namespace ui
{
    class command_input;
    class window;
} // ui

namespace ui::interfaces::light
{
    class button;

    class access_bar : public ui::element
    {
    public:
        access_bar(ui::user&, ui::window*);

    private:
        ui::user& user_;
    };
} // ui::interfaces::light

#endif // INCLUDE_UI_INTERFACE_LIGHT_ACCESS_BAR_HPP_NXI