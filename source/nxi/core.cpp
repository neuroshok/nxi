#include <nxi/core.hpp>

#include <nxi/log.hpp>

#include <QCoreApplication>
#include <QDir>

namespace nxi
{
    core::core()
        : ndb_init_{}
        , command_system_{ *this }
        , context_system_{ *this }
        , interface_system_{ *this }
        , module_system_{ *this }
        , page_system_{ *this }
        , window_system_{ *this }
        , api_{ *this }
        , config_{ "nxi" }
    {}

    core::~core() = default;

    void core::load()
    {
        nxi_trace("");

        window_system_.load(); // window create interface

        command_system_.load();
        page_system_.load();
        interface_system_.load();
        context_system_.load();

        // todo session system
        // session_system_.load();

        module_system_.load();
    }

    void core::quit() const
    {
        emit event_quit();
    }

    nxi::api::core& core::api() { return api_; }
    nxi::config& core::config() { return config_; }

    nxi::command_system& core::command_system() { return command_system_; }
    nxi::context_system& core::context_system() { return context_system_; }
    nxi::interface_system& core::interface_system() { return interface_system_; }
    nxi::module_system& core::module_system() { return module_system_; }
    nxi::page_system& core::page_system() { return page_system_; }
    nxi::window_system& core::window_system() { return window_system_; }

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
