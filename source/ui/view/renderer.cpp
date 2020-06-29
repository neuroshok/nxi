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
    renderer_view::renderer_view(ui::session& session, QWidget* parent)
        : QWidget(parent)
        , session_{ session }
    {
        layout_ = new nxw::vbox_layout;
        setLayout(layout_);
    }

    void renderer_view::display(const nxi::page& page)
    {
        nxi_trace("display {}", page.name());
        auto ui_page = session_.page_system().get(page);

        auto get_renderer = [this, &ui_page](const nxi::page& page)
        {
            for (auto renderer : renderers_) renderer->hide();
            for (auto renderer : renderers_)
            {
                // get the first available renderer of the right type
                if (renderer->type() == page.renderer_type()) return renderer.get();
            }
            // not renderer available, make one
            renderers_.push_back(stz::make_observer<ui::renderer>(ui::renderer::make(ui_page)));
            return renderers_.back().get();
        };

        ui::renderer* page_renderer = get_renderer(page);
        page_renderer->show();
        // if mode == multi, new ui::node_renderer, page.display(node_renderer)

        while (layout_->takeAt(0)) {}
        layout_->addWidget(page_renderer);

        ui_page->display(page_renderer);
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
} // ui