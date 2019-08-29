#include <ui/system/page.hpp>

#include <nxi/core.hpp>
#include <nxi/page/custom.hpp>
#include <nxi/page/node.hpp>

#include <ui/core.hpp>
#include <ui/page.hpp>
#include <ui/page/web.hpp>
#include <ui/page/widget.hpp>

#include <QObject>
#include <include/nxi/log.hpp>
#include <include/ui/view/config.hpp>

namespace ui
{
    page_system::page_system(ui::core& ui_core) : ui_core_{ ui_core }
    {
        // init static widget page
        make_widget<ui::views::config>("nxi/config");

        QObject::connect(&ui_core_.nxi_core().page_system(), &nxi::page_system::event_add, this, [this](nxi::page& page, nxi::page_id)
        {
            if (page.renderer_type() == nxi::renderer_type::web) pages_.emplace(page.id(), std::make_unique<ui::web_page>(ui_core_, static_cast<nxi::web_page&>(page)));
            else if (page.renderer_type() == nxi::renderer_type::widget) pages_.emplace(page.id(), std::make_unique<ui::widget_page>(ui_core_, static_cast<nxi::custom_page&>(page)));
        });
    }

    stz::observer_ptr<ui::page> page_system::get(nxi::page& page)
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