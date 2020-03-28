#include <nxi/page/web.hpp>

#include <nxi/core.hpp>
#include <nxi/config.hpp>
#include <nxi/system/page.hpp>

namespace nxi
{


    web_page::web_page(nds::node_ptr<nxi::page> page_ptr, nxi::page_system& ps)
        : web_page(page_ptr, ps, ps.nxi_core_.config().browser.home.get().c_str())
    {}

    web_page::web_page(nds::node_ptr<nxi::page> page_ptr, nxi::page_system& ps, const QString& url)
        : nxi::page(page_ptr, ps, "new_page", url, nxi::page_type::web, nxi::renderer_type::web)
    {}

} // nxi