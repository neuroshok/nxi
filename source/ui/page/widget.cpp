#include <ui/page/widget.hpp>

#include <nxi/page/custom.hpp>

#include <nxi/page/custom.hpp>

#include <nxw/hbox_layout.hpp>
#include <ui/core.hpp>
#include <ui/renderer/widget.hpp>
#include <ui/user.hpp>

namespace ui
{
    widget_page::widget_page(ui::user& user, nxi::custom_page& page)
        : ui::page{ page }
        , page_{ page }
        , user_{ user }
        , widget_{ user_.page_system().get(page.command()) }
    {
        auto* l = new nxw::hbox_layout;
        setLayout(l);
        l->addWidget(widget_);
    }

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