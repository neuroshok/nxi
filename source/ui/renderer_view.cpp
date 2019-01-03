#include <ui/view/renderer.hpp>

#include <ui/core.hpp>
#include <ui/system/page.hpp>
#include <nxi/core.hpp>
#include <nxi/page/node.hpp>

#include <include/ui/renderer/web.hpp>
#include <ui/page.hpp>
#include <nxw/vbox_layout.hpp>
#include <nxi/system/page.hpp>
#include <include/ui/renderer/qt3d.hpp>

namespace ui
{
    renderer_view::renderer_view(ui::core& ui_core, QWidget* parent)
        : QWidget(parent)
        , ui_core_{ ui_core }
        , renderer_{ nullptr }
    {
        layout_ = new nxw::vbox_layout;
        setLayout(layout_);

        QObject::connect(&ui_core.nxi_core().page_system(), qOverload<nxi::page&>(&nxi::page_system::event_focus), this, [this](nxi::page& page)
        {
            display(page);
        });

        QObject::connect(&ui_core.nxi_core().page_system(), qOverload<nxi::page_node&>(&nxi::page_system::event_focus), this, [this](nxi::page_node& node)
        {
            qDebug() << "event_focus node" << node.name();

            delete renderer_;
            renderer_ = new ui::qt3d_renderer;
            while (layout_->takeAt(0)) {}
            layout_->addWidget(renderer_->widget());

            //display(ui_core_.nxi_core().page_system().list(node));
        });
    }

    void renderer_view::display(nxi::page& page)
    {
        auto ui_page = ui_core_.page_system().get(page);

        // update renderer
        delete renderer_;
        renderer_ = ui_page->make_renderer();
        while (layout_->takeAt(0)) {}
        layout_->addWidget(renderer_->widget());

        ui_page->display(renderer_);
        //page->displayable_with(renderer_);
    }

    void renderer_view::display(nxi::page_system::pages_view pages)
    {
        for (auto page : pages)
        {
            auto ui_page = ui_core_.page_system().get(*page);
            auto renderer = ui_page->make_renderer();
            ui_page->display(renderer);
            layout_->addWidget(renderer->widget());
        }
    }

} // ui

// REPLACE ui::page by nxi::page, get ui_page in renderers