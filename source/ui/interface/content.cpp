#include <ui/interface/content.hpp>

#include <ui/core.hpp>
#include <nxi/core.hpp>
#include <nxi/system/command.hpp>

#include <nxw/hbox_layout.hpp>
#include <ui/view/web.hpp>
#include <ui/view/explorer.hpp>

#include <QStackedWidget>
#include <QWebEngineView>

#include <ui/view/renderer.hpp>
#include <include/nxi/log.hpp>

namespace ui::interfaces
{
    content::content(ui::core& ui_core) :
        ui_core_{ ui_core }
    {
        QHBoxLayout* layout = new nxw::hbox_layout;
        setLayout(layout);

        renderer_view_ = new ui::renderer_view(ui_core_, this);
        layout->addWidget(renderer_view_);

        // connect(page_focus) render_view_->focus(page)
        connect(&ui_core_.nxi_core().page_system(), qOverload<nxi::page&>(&nxi::page_system::event_focus), [this](nxi::page& page)
        {
            nxi_trace("focus page {}", page.name());

            renderer_view_->display(page);
        });

        connect(&ui_core_.nxi_core().page_system(), qOverload<nxi::page&, nxi::page_id>(&nxi::page_system::event_add), [](nxi::page& page,  nxi::page_id)
        {
            nxi_trace("add page {}", page.name());
        });


    }
} // ui::interfaces