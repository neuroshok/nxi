#ifndef INCLUDE_UI_ELEMENT_HPP_NXI
#define INCLUDE_UI_ELEMENT_HPP_NXI

#include <ui/window.hpp>

namespace ui
{
    template<class Widget>
    class basic_element : public Widget
    {
    public:
        using Widget::Widget;

        ui::window* ui_window() { return static_cast<ui::window*>(Widget::window()); }
        [[nodiscard]] const ui::window* ui_window() const { return static_cast<ui::window*>(Widget::window()); }
        [[nodiscard]] int group_id() const { return ui_window()->id(); }
    };

    using element = basic_element<QWidget>;
} // ui

#endif // INCLUDE_UI_ELEMENT_HPP_NXI