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
    content::content(ui::user_session& session, ui::window* window)
        : ui::interface("content", window)
        , session_{ session }
    {
        layout_ = new nxw::hbox_layout;
        setLayout(layout_);

        focus_ = add();
        add();

        connect(&session_.nxi_session().buffer_system().group(ui_window()->id()), &nxi::buffer_group::event_buffer_focus,
                [this](nxi::buffer& buffer) {
                    nxi_assert(views_.find(buffer.id()) != views_.end());
                    focus_ = views_[buffer.id()];
                });

        connect(&session_.nxi_session().buffer_system().group(ui_window()->id()), &nxi::buffer_group::event_page_activate,
                [this](nxi::page_system::page_ptr page) { focus_->display(page); });
    }

    ui::renderer_view* content::add()
    {
        auto renderer_view = new ui::renderer_view(session_, this);
        layout_->addWidget(renderer_view);
        views_.emplace(renderer_view->buffer().id(), renderer_view);
        return renderer_view;
    }
} // ui::interfaces