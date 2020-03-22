#include <ui/system/page.hpp>

#include <nxi/core.hpp>
#include <nxi/page/custom.hpp>
#include <nxi/page/node.hpp>

#include <ui/core.hpp>
#include <ui/page.hpp>
#include <ui/page/node.hpp>
#include <ui/page/web.hpp>
#include <ui/page/widget.hpp>

#include <QObject>
#include <include/nxi/log.hpp>
#include <ui/view/config.hpp>
#include <ui/view/aboutgl.hpp>

namespace ui
{
    page_system::page_system(ui::core& ui_core) : ui_core_{ ui_core }
    {
        // todo rename views to pages
        // init static widget page
        make_widget<ui::views::config>("nxi/config");
        make_widget<ui::views::aboutgl>("nxi/aboutgl");

        connect(&ui_core_.nxi_core().page_system(), &nxi::page_system::event_add, this, [this](nxi::page_system::page_ptr page, nxi::page_system::page_ptr)
        {
            // todo replace by page->make_ui(this);
            if (page->type() == nxi::page_type::node) pages_.emplace(page->id(), std::make_unique<ui::node_page>(ui_core_, static_cast<nxi::page_node&>(*page)));
            else if (page->renderer_type() == nxi::renderer_type::web) pages_.emplace(page->id(), std::make_unique<ui::web_page>(ui_core_, static_cast<nxi::web_page&>(*page)));
            else if (page->renderer_type() == nxi::renderer_type::widget) pages_.emplace(page->id(), std::make_unique<ui::widget_page>(ui_core_, static_cast<nxi::custom_page&>(*page)));
            else nxi_error("fail");
        });
    }


    stz::observer_ptr<ui::page> page_system::get(const nxi::page& page)
    {
        auto it = pages_.find(page.id());
        nxi_assert(it != pages_.end());
        return stz::make_observer(it->second.get());
    }

    QWidget* page_system::get(const QString& page_path)
    {
        auto it = widget_pages_.find(page_path);
        nxi_assert(it != widget_pages_.end());
        return it.value();
    }
} // ui