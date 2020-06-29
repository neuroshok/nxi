#include <nxi/module/web.hpp>

#include <nxi/command.hpp>
#include <nxi/core.hpp>
#include <nxi/log.hpp>
#include <nxi/module/web.hpp>
#include <nxi/page/web.hpp>
#include <nxi/system/command.hpp>
#include <nxi/utility/file.hpp>
#include <w3c/module/manifest.hpp>

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#include <QWebEngineScript>

#include <QDebug>

namespace nxi
{

    web_module::web_module(nxi::session& session, const QString& name)
        : module(name, module_type::web)
        , session_{ session }
        , manifest_ { name }
    {
        init_scripts();
    }

    void web_module::on_load()
    {
        // add module commands
        auto f = [this](const nxi::values&)
        {
            if (manifest_.browser_action.default_popup.isEmpty()) session_.error("module has no page");
            else
            {
                auto page_path = nxi::core::module_path(name(), nxi::module_type::web) + "/" + manifest_.browser_action.default_popup;
                session_.page_system().open<nxi::web_page>("file:///" + page_path);
            }
        };
        nxi::command command{ name(), "main", f, nxi::core::module_path(name(), nxi::module_type::web) + browser_action_.default_icon };
        session_.command_system().add(std::move(command));
    }

    void web_module::process(nxi::web_page& page)
    {
        nxi_trace("process page {}", page.command());

        for (const auto& content_script : manifest_.content_scripts)
        {
            // if match
            for (const auto& js_source : content_script.js) page.add_script(scripts_[js_source]);
        }
    }

    void web_module::init_scripts()
    {
        for (const auto& content_script : manifest_.content_scripts)
        {
            for (const auto& js_source_path : content_script.js)
            {
                QWebEngineScript script;
                script.setName(js_source_path);
                script.setRunsOnSubFrames(false);
                script.setSourceCode(nxi::read_file(nxi::core::module_path(name(), nxi::module_type::web) + "/" + js_source_path));
                using run_at_type = w3c::manifest::content_scripts_t::run_at_type;
                if (content_script.run_at == run_at_type::document_idle) script.setInjectionPoint(QWebEngineScript::DocumentReady);
                script.setWorldId(QWebEngineScript::MainWorld);
                scripts_.insert(js_source_path, std::move(script));
            }
        }
    }
} // nxi