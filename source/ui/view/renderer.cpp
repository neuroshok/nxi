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
#include <include/nxi/log.hpp>

namespace ui
{
    renderer_view::renderer_view(ui::core& ui_core, QWidget* parent)
        : QWidget(parent)
        , ui_core_{ ui_core }
        , renderer_{ nullptr }
    {
        layout_ = new nxw::vbox_layout;
        setLayout(layout_);
    }

    void renderer_view::display(nxi::page& page)
    {
        nxi_trace("display {}", page.name());
        auto ui_page = ui_core_.page_system().get(page);

        // update renderer
        //delete renderer_;

        renderer_ = ui::renderer::make(ui_page);

        while (layout_->takeAt(0)) {}
        layout_->addWidget(renderer_->widget());


        ui_page->display(renderer_);
        //ui_page->get_renderer(renderer_);
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