#include <ui/page/widget.hpp>

#include <nxi/page/custom.hpp>

#include <nxi/page/custom.hpp>

#include <ui/core.hpp>
#include <ui/renderer/widget.hpp>

namespace ui
{
    widget_page::widget_page(ui::core& ui_core, nxi::custom_page& page)
        : ui::page{ page }
        , page_{ page }
        , ui_core_{ ui_core }
        , widget_{ ui_core_.page_system().get(page.command()) }
    {}

    void widget_page::display(ui::renderer *renderer)
    {
        renderer->display(this);
    }

    ui::renderer* widget_page::make_renderer() const
    {
        return new widget_renderer;
    }

    QWidget* widget_page::widget() { return widget_; }
} // ui