#include <nxi/page/web.hpp>

#include <nxi/core.hpp>
#include <nxi/config.hpp>
#include <nxi/system/page.hpp>

#include <QWebEngineScript>

namespace nxi
{
    web_page::web_page(nds::node_ptr<nxi::page> page_ptr, nxi::page_system& ps)
        : web_page(page_ptr, ps, "http://nxi.neuroshok.com")
    {}

    web_page::web_page(nds::node_ptr<nxi::page> page_ptr, nxi::page_system& ps, const QString& url)
        : nxi::page(page_ptr, ps, "new_page", url, nxi::page_type::web, nxi::renderer_type::web)
    {}

    void web_page::add_script(const QWebEngineScript& script) const
    {
        emit event_add_script(script);
    }

    void web_page::run_script(const QString& source_code) const
    {
        emit event_run_script(source_code);
    }

    void web_page::run_script(const QString& source_code, std::function<void(const QVariant&)> fn) const
    {
        emit event_call_script(source_code, fn);
    }
} // nxi