#include <nxi/core.hpp>

#include <nxi/log.hpp>

#include <filesystem>

#include <QCoreApplication>
#include <QDir>
#include <QWebEngineProfile>
#include <QWebEngineSettings>

namespace fs = std::filesystem;

namespace nxi
{
    core::core()
        : global_database_{}
        , api_{ *this } // init before systems
        , config_{}
        , command_system_{ *this }
        , context_system_{ *this }
        , interface_system_{ *this }
        , module_system_{ *this }
        , page_system_{ *this }
        , session_system_{ *this }
        , window_system_{ *this }
    {
        // apply config
        QWebEngineProfile::defaultProfile()->settings()->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled, true);
        QWebEngineProfile::defaultProfile()->settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);
        QWebEngineProfile::defaultProfile()->setHttpUserAgent("Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:10.0) Gecko/20100101 Firefox/74.0.1");
    }

    core::~core() = default;

    void core::load()
    {
        nxi_trace("");

        global_database_.connect();

        session_system_.load();

        /*
        window_system_.load(); // window create interface

        command_system_.load();
        page_system_.load();
        interface_system_.load();
        context_system_.load();

        module_system_.load();*/
    }

    void core::quit() const
    {
        emit event_quit();
    }

    nxi::api::core& core::api() { return api_; }
    nxi::config& core::config() { return config_; }
    nxi::database& core::database() { return session_system_.focus()->core_database(); }
    nxi::database& core::global_database() { return global_database_; }

    nxi::command_system& core::command_system() { return command_system_; }
    nxi::context_system& core::context_system() { return context_system_; }
    nxi::interface_system& core::interface_system() { return interface_system_; }
    nxi::module_system& core::module_system() { return module_system_; }
    nxi::page_system& core::page_system() { return page_system_; }
    nxi::session_system& core::session_system() { return session_system_; }
    nxi::window_system& core::window_system() { return session_system_.focus()->window_system(); }

    void core::error(const QString& message) const
    {
        emit event_error(message);
    }

    QString core::module_path()
    {
        return QCoreApplication::applicationDirPath() + "/module";
    }

    QString core::module_path(const QString& name, nxi::module_type module_type)
    {
        QString path;
        switch(module_type)
        {
            case nxi::module_type::web:
                path = module_path() +"/webextension/" + name;
                break;
            case nxi::module_type::dynamic:
                path = module_path() +"/dynamic/" + name;
                break;
            default:
                nxi_warning("unknown module type");
        }
        return path;
    }

    QString core::page_path(const QString& path)
    {
        return "qrc:/page/" + path;
    }
} // nxi
