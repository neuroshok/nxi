#include "adk.hpp"

#include <nxi/core.hpp>
#include <nxi/system/window.hpp>
#include <nxi/log.hpp>
#include <nxi/page/web.hpp>

#include <QWebEngineUrlRequestInterceptor>
#include <QWebEngineUrlRequestInfo>
#include <QWebEngineProfile>
#include <ui/core.hpp>

struct interceptor : QWebEngineUrlRequestInterceptor
{
    virtual void interceptRequest(QWebEngineUrlRequestInfo& info) override
    {
        //qDebug() << info.requestUrl();
        if (info.requestUrl().toString().contains("adsbygoogle")) info.block(true);
    }
};

namespace nxi::modules
{
    adk::adk(nxi::session& session)
        : module("adk", module_type::compiled)
        , session_{ session }
        , interceptor_{ new ::interceptor }
    {
    }

    void adk::on_load()
    {
        QWebEngineProfile::defaultProfile()->setUrlRequestInterceptor(interceptor_);

        auto cmd = nxi::command("adk", "main", [this](const nxi::values&)
        {
            session_.page_system().open<nxi::web_page>(nxi::core::module_path() + "/adk.html");
        });

        session_.command_system().add(std::move(cmd) /*, this (source) */ );
    }

    void adk::on_unload()
    {
        QWebEngineProfile::defaultProfile()->setUrlRequestInterceptor(nullptr);


        // session_.command_system().del(this); // delete all commands from this module
    }

} // nxi::modules