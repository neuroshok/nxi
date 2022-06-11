#include <ui/system/page.hpp>

#include <nxi/core.hpp>
#include <nxi/page/custom.hpp>
#include <nxi/page/node.hpp>
#include <nxi/page/web.hpp>

#include <ui/core.hpp>
#include <ui/page.hpp>
#include <ui/page/node.hpp>
#include <ui/page/web.hpp>
#include <ui/page/widget.hpp>

#include <QObject>
#include <nxi/log.hpp>
#include <ui/view/config.hpp>
#include <ui/view/aboutgl.hpp>

namespace ui
{
    page_system::page_system(ui::user& user)
        : user_{ user }
    {
        // todo rename views to pages
        // init static widget page
        make_widget<ui::views::config>("nxi/config");
        // make_widget<ui::views::aboutgl>("nxi/aboutgl");

        connect(&user_.nxi_user().page_system(), &nxi::page_system::event_add, this, [this](nds::node_ptr<nxi::page> page, nds::node_ptr<nxi::page>) {
            // todo replace by page->make_ui(this);
            if (page->type() == nxi::page_type::node) pages_.emplace(page->id(), QPointer{ new ui::node_page(user_, page) });
            else if (page->renderer_type() == nxi::renderer_type::web)
                pages_.emplace(page->id(), QPointer{ new ui::web_page(user_, static_cast<nxi::web_page&>(*page)) });
            else if (page->renderer_type() == nxi::renderer_type::widget)
                pages_.emplace(page->id(), QPointer{ new ui::widget_page(user_, static_cast<nxi::custom_page&>(*page)) });
            else nxi_error("fail");
        });

        connect(&user_.nxi_user().page_system(), &nxi::page_system::event_close, this, [this](nds::node_ptr<nxi::page> page) {
            auto it = pages_.find(page->id());
            nxi_assert(it != pages_.end());
            delete it->second;
            pages_.erase(it);
        });
    }

    page_system::~page_system()
    {
        for (auto [id, page_ptr] : pages_)
        {
            // check if page has been deleted by Qt
            if (page_ptr) delete page_ptr;
        }
        // todo delete widget_pages ?
    }

    stz::observer_ptr<ui::page> page_system::get(const nxi::page& page)
    {
        auto it = pages_.find(page.id());
        nxi_assert(it != pages_.end() && it->second.data());
        return stz::make_observer(it->second.data());
    }

    QWidget* page_system::get(const QString& page_path)
    {
        auto it = widget_pages_.find(page_path);
        nxi_assert(it != widget_pages_.end());
        return it.value();
    }
} // ui