#include <nxi/core.hpp>

#include <nxi/log.hpp>

#include <QCoreApplication>
#include <QDir>

namespace nxi
{
    core::core()
        : global_database_{}
        , config_{ nullptr }
        , session_system_{ *this }
    {}

    void core::load()
    {
        nxi_trace("");

        global_database_.connect();
        config_ = std::make_unique<nxi::config>("nxi", global_database_);

        session_system_.load();
        emit event_load();
    }

    void core::quit() const
    {
        emit event_quit();
    }

    nxi::config& core::config()
    {
        nxi_assert(config_ != nullptr);
        return *config_;
    }

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