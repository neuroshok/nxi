#include <nxi/page/web.hpp>

#include <nxi/core.hpp>
#include <nxi/config.hpp>
#include <nxi/system/page.hpp>

namespace nxi
{
    web_page::web_page(nds::node_ptr<nxi::page> page_ptr, nxi::page_system& ps)
        : nxi::page(page_ptr, ps, "web_page", ps.nxi_core_.config().browser.home.get().c_str(), nxi::page_type::web, nxi::renderer_type::web)
    {}


} // nxi