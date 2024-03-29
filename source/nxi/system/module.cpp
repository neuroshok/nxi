#include <nxi/system/module.hpp>

#include <nxi/core.hpp>
#include <nxi/data/module.hpp>
#include <nxi/log.hpp>
#include <nxi/module/binary.hpp>
#include <nxi/module/web.hpp>
#include <nxi/type.hpp>
#include <nxi/utility/file.hpp>
#include <nxi/web_session.hpp>

#include <QWebChannel>
#include <QWebEngineProfile>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>

// include modules
#include <nxi/module/static.generated.hpp>

namespace nxi
{
    module_system::module_system(nxi::core& core)
        : core_{ core }
        , static_modules_{ core_ }
        , web_channel_{ new QWebChannel{ this } }
    {}

    const std::vector<std::unique_ptr<nxi::module>>& module_system::get() const { return modules_; }

    void module_system::load()
    {
        nxi_trace("");

        web_channel_->registerObject("core", &core_.user().api());
        web_channel_->registerObject("page_system", &core_.user().api().page_system());

        nxi_trace("load js_api");
        js_api_ = nxi::read_file(":/qtwebchannel/qwebchannel.js");
        js_api_.append(nxi::read_file(":/script/js/nxi"));

        QWebEngineScript script;
        script.setSourceCode(js_api_);
        script.setInjectionPoint(QWebEngineScript::DocumentCreation);
        script.setWorldId(QWebEngineScript::MainWorld);
        // core_.web_session().scripts()->insert(script);

        nxi_trace("load static modules");
        static_modules_.load();

        nxi_trace("load db modules");

        auto result = nxi::data::module::get(core_.user_database());
        while(result.next())
        {
            std::unique_ptr<nxi::module> module_ptr;
            auto module_data = nxi::data::module::from_get(result);
            auto module_type = module_data.type;

            if (!module_data.active) continue;

            switch(module_type)
            {
            case nxi::module_type::compiled:
                // all modules are loaded by default
                break;

            case nxi::module_type::dynamic:
                module_ptr = std::make_unique<nxi::dynamic_module>(core_, module_data.name);
                module_ptr->load();
                modules_.push_back(std::move(module_ptr));
                break;

            case nxi::module_type::web:
                module_ptr = std::make_unique<nxi::web_module>(core_, module_data.name);
                module_ptr->load();
                modules_.push_back(std::move(module_ptr));
                break;

            default:
                nxi_error("unknokwn module type");
                break;
            }
            //if (module_data.loaded) emit event_load();
        }
    }

    void module_system::process(nxi::web_page& page)
    {
        for (auto& module : modules_)
        {
            module->process(page);
        }
    }

    const QString& module_system::js_api() const
    {
        return js_api_;
    }

    QWebChannel* module_system::web_channel() const
    {
        return web_channel_;
    }
} // nxi