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
        , config_{}
        , session_system_{ *this }
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
    }

    void core::quit() const
    {
        emit event_quit();
    }

    nxi::config& core::config() { return config_; }
    nxi::database& core::global_database() { return global_database_; }

    nxi::session_system& core::session_system() { return session_system_; }

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
