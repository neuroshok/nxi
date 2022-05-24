#ifndef INCLUDE_UI_ELEMENT_HPP_NXI
#define INCLUDE_UI_ELEMENT_HPP_NXI

#include <ui/window.hpp>

namespace ui
{
    class element
    {
    public:
        element();

    private:
    };

    template<class Widget>
    class basic_element : public Widget
    {
    public:
        using Widget::Widget;
        ui::window* ui_window() { return static_cast<ui::window*>(Widget::window()); }
    };
} // ui

#endif // INCLUDE_UI_ELEMENT_HPP_NXI