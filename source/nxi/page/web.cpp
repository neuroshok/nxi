#include <nxi/page/web.hpp>

#include <include/nxi/database/config.hpp>
#include <nxi/core.hpp>
#include <nxi/database/config.hpp>
#include <nxi/system/page.hpp>

namespace nxi
{
    web_page::web_page(nxi::page_system& ps, nxi::page_id id)
        : nxi::page(ps, id, "web_page")
        , url_{ ps.nxi_core_.config().browser.home.get().c_str() }
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