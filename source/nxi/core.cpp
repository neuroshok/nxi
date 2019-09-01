#include <nxi/core.hpp>

#include <nxi/log.hpp>

#include <QCoreApplication>
#include <QDir>

namespace nxi
{
    core::core()
        : ndb_init_{}
        , command_system_{ *this }
        , window_system_{ *this }
        , page_system_{ *this }
        , module_system_{ *this }
        , config_{ "nxi" }
    {}

    core::~core() = default;

    void core::load()
    {
        nxi_trace("");

        // load modules before other systems
        module_system_.load();

        command_system_.load();
        window_system_.load();
        page_system_.load();
    }

    void core::quit() const
    {
        emit event_quit();
    }

    nxi::command_system& core::command_system()
    {
        return command_system_;
    }

    nxi::module_system& core::module_system()
    {
        return module_system_;
    }

    nxi::page_system& core::page_system()
    {
        return page_system_;
    }

    nxi::window_system& core::window_system()
    {
        return window_system_;
    }

    void core::error(const QString& message) const
    {
        emit event_error(message);
    }

    QString core::page_path(const QString& path)
    {
        return "qrc:/page/" + path;
    }
} // nxi
