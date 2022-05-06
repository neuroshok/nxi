#include <nxi/core.hpp>

#include <nxi/log.hpp>
#include <nxi/user.hpp>

#include <QCoreApplication>
#include <QDir>

namespace nxi
{
    core::core()
        : global_database_{}
        , user_system_{ *this }
    {}

    void core::load()
    {
        nxi_trace("");

        global_database_.connect();

        user_system_.load();
        emit event_load();
    }

    void core::error(const QString& message) { user().error(message); }
    void core::quit() const { emit event_quit(); }

    nxi::database& core::global_database() { return global_database_; }
    nxi::user_system& core::user_system() { return user_system_; }

    nxi::session& core::session() { return user().session_system().focus(); }
    nxi::user& core::user() { return user_system_.focus(); }

    nxi::config& core::session_config() { return user().session_system().focus().config(); }

    nxi::command_system& core::command_system() { return user().command_system(); }
    nxi::context_system& core::context_system() { return user().context_system(); }
    nxi::interface_system& core::interface_system() { return user().interface_system(); }
    nxi::navigation_system& core::navigation_system() { return user().navigation_system(); }
    nxi::page_system& core::page_system() { return user().page_system(); }
    nxi::session_system& core::session_system() { return user().session_system(); }

    nxi::database& core::user_database() { return user().database(); }

    QString core::module_path() { return QCoreApplication::applicationDirPath() + "/module"; }

    QString core::module_path(const QString& name, nxi::module_type module_type)
    {
        QString path;
        switch (module_type)
        {
        case nxi::module_type::web:
            path = module_path() + "/webextension/" + name;
            break;
        case nxi::module_type::dynamic:
            path = module_path() + "/dynamic/" + name;
            break;
        default:
            nxi_warning("unknown module type");
        }
        return path;
    }

    QString core::page_path(const QString& path) { return "qrc:/page/" + path; }
} // nxi