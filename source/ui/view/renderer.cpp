#include <ui/view/renderer.hpp>

#include <nxi/core.hpp>
#include <nxi/page/node.hpp>
#include <nxi/window.hpp>
#include <ui/core.hpp>
#include <ui/system/page.hpp>
#include <ui/window.hpp>

#include <nxi/log.hpp>
#include <nxi/system/page.hpp>
#include <nxw/vbox_layout.hpp>
#include <ui/page.hpp>
#include <ui/renderer/qt3d.hpp>
#include <ui/renderer/web.hpp>
#include <QLabel>

namespace ui
{
    renderer_view::renderer_view(ui::user_session& session, QWidget* parent)
        : ui::basic_element<QWidget>(parent)
        , session_{ session }
        , renderer_{ new ui::web_renderer }
        , buffer_{ session_.nxi_session().buffer_system().add(group_id()) }
    {
        renderer_->widget()->setParent(this);

        layout_ = new nxw::vbox_layout;
        setLayout(layout_);
        setFocusPolicy(Qt::FocusPolicy::ClickFocus);
        focus_marker_ = new QLabel{ this };
        focus_marker_->setFixedHeight(5);
        focus_marker_->setStyleSheet("background-color: transparent");
        layout_->addWidget(focus_marker_, 0, Qt::AlignTop);
        layout_->addWidget(renderer_->widget(), 1);
    }

    void renderer_view::display(nxi::page_system::page_ptr page)
    {
        nxi_trace("renderer_view::display {}", page->name());

        buffer_.set_page(page);

        auto ui_page = session_.page_system().get(*page);

        if (renderer_->type() != page->renderer_type())
        {
            delete renderer_;
            renderer_ = ui::renderer::make(ui_page);
            renderer_->widget()->setParent(this);
            renderer_->widget()->show();
            layout_->addWidget(renderer_->widget(), 1);
        }

        ui_page->display(renderer_);
    }

    void renderer_view::display(const nxi::page_system::pages_view pages)
    {
        /*
        for (auto page : pages)
        {
            auto ui_page = ui_core_.page_system().get(*page);
            auto renderer = ui_page->make_renderer();
            ui_page->display(renderer);
            layout_->addWidget(renderer->widget());
        }
         */
    }

    void renderer_view::focusInEvent(QFocusEvent*)
    {
        session_.nxi_session().buffer_system().focus(buffer_);
        // session_.nxi_session().buffer_system().group(group_id()).focus(buffer_);

        focus_marker_->setStyleSheet("background-color: " +
                                     session_.nxi_session().interface_system().style().data().menu.item_background_color_selected.get().name());
    }

    void renderer_view::focusOutEvent(QFocusEvent*) { focus_marker_->setStyleSheet("background-color: transparent"); }
} // ui