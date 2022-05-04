#include <ui/page/widget.hpp>

#include <nxi/page/custom.hpp>

#include <nxi/page/custom.hpp>


#include <ui/user_session.hpp>
#include <ui/core.hpp>
#include <ui/renderer/widget.hpp>
#include <nxw/hbox_layout.hpp>

namespace ui
{
    widget_page::widget_page(ui::user_session& session, nxi::custom_page& page)
        : ui::page{ page }
        , page_{ page }
        , session_{ session }
        , widget_{ session_.page_system().get(page.command()) }
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