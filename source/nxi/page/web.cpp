#include <nxi/page/web.hpp>

#include <nxi/core.hpp>
#include <nxi/config.hpp>
#include <nxi/system/page.hpp>

#include <QWebEngineScript>

namespace nxi
{
    web_page::web_page(nds::node_ptr<nxi::page> page_ptr, nxi::page_system& ps, nxi::page_data data)
        : nxi::page(page_ptr, ps, std::move(data))
    {}

    web_page::web_page(nds::node_ptr<nxi::page> page_ptr, nxi::page_system& ps, QString url)
        : web_page(page_ptr, ps, nxi::page_data{ 0, url, std::move(url), nxi::page_type::web, nxi::renderer_type::web, ps.session_id() })
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