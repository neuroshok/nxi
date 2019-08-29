#include <nxi/page/web.hpp>

#include <nxi/core.hpp>
#include <nxi/config.hpp>
#include <nxi/system/page.hpp>

namespace nxi
{
    web_page::web_page(nxi::page_system& ps)
        : nxi::page(ps, "web_page", ps.nxi_core_.config().browser.home.get().c_str(), nxi::page_type::web, nxi::renderer_type::web)
    {}

    void web_page::focus()
    {
        page_system_.focus(*this);
    }

    void web_page::load()
    {
        emit event_load();
        page_system_.load(*this);
    }
} // nxi