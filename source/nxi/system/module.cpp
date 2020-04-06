#include <nxi/system/module.hpp>

#include <nxi/core.hpp>
#include <nxi/database/module.hpp>
#include <nxi/log.hpp>
#include <nxi/module/binary.hpp>
#include <nxi/module/web.hpp>
#include <nxi/utility/file.hpp>

#include <QWebChannel>
#include <QWebEngineProfile>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>

// include modules
#include <nxi/module/static.generated.hpp>

namespace nxi
{
    module_system::module_system(nxi::core& nxi_core) :
		nxi_core_{ nxi_core }
		, static_modules_{ nxi_core_ }
	{}

    const std::vector<std::unique_ptr<nxi::module>>& module_system::get() const
    {
        return modules_;
    }

    void module_system::load()
    {
        nxi_trace("");

        nxi_trace("load js_api");
        js_api_ = nxi::read_file(":/qtwebchannel/qwebchannel.js");
        js_api_.append(nxi::read_file(":/script/js/nxi"));

        QWebEngineScript script;
        script.setSourceCode(js_api_);
        script.setInjectionPoint(QWebEngineScript::DocumentCreation);
        script.setWorldId(QWebEngineScript::MainWorld);
        QWebEngineProfile::defaultProfile()->scripts()->insert(script);

        nxi_trace("load static modules");
        static_modules_.load();

        nxi_trace("load db modules");
        for (auto& module : ndb::oget<dbs::core>(nxi_model.module))
        {
            std::unique_ptr<nxi::module> module_ptr;

            switch(module.type)
            {
                case nxi::module_type::compiled:
                    // all modules are loaded by default
                    break;

                case nxi::module_type::dynamic:
                    module_ptr = std::make_unique<nxi::dynamic_module>(nxi_core_, module.name);
                    nxi_trace("load {}", module_ptr->name());
                    module_ptr->load();
                    modules_.push_back(std::move(module_ptr));
                    break;

                case nxi::module_type::web:
                    module_ptr = std::make_unique<nxi::web_module>(nxi_core_, module.name);
                    nxi_trace("load {}", module_ptr->name());
                    module_ptr->load();
                    modules_.push_back(std::move(module_ptr));
                    break;

                default:
                    nxi_error("unknokwn module type");
            }

            // if (state == enable)
        }
        //emit event_load(module_name);

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
} // nxi