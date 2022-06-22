#include <ui/interface/standard/content.hpp>

#include <nxi/core.hpp>
#include <nxi/log.hpp>
#include <nxi/system/command.hpp>
#include <nxw/hbox_layout.hpp>

#include <ui/core.hpp>
#include <ui/view/explorer.hpp>

#include <ui/view/renderer.hpp>
#include <QApplication>
#include <QStackedWidget>
#include <QWebEngineView>

namespace ui::interfaces::standard
{
    content::content(ui::user& user, ui::window* window)
        : ui::interface("content", window)
        , user_{ user }
    {
        layout_ = new nxw::hbox_layout;
        setLayout(layout_);

        focus_ = add_view();

        connect(&user_.nxi_user().buffer_system().group(ui_window()->id()), &nxi::buffer_group::event_buffer_focus, this,
                [this](nxi::buffer& buffer) {
                    nxi_assert(views_.find(buffer.id()) != views_.end());
                    focus_ = views_[buffer.id()];
                });

        connect(&user_.nxi_user().buffer_system().group(group_id()), &nxi::buffer_group::event_page_activate, this,
                [this](nxi::page_system::page_ptr page) { focus_->display(page); });
    }

    ui::renderer_view* content::add_view()
    {
        auto renderer_view = new ui::renderer_view(user_, this);
        layout_->addWidget(renderer_view);
        views_.emplace(renderer_view->buffer().id(), renderer_view);
        return renderer_view;
    }
} // ui::interfaces