#include <nxi/session.hpp>

namespace nxi
{
    session::session(nxi::core& core, nxi::session_data data)
        : nxi_core_{ core }
        , name_{ std::move(data.name) }
        , active_{ std::move(data.active) }
        , command_system_{ nxi_core_ }
        , context_system_{ nxi_core_ }
        , interface_system_{ nxi_core_ }
        , window_system_{ nxi_core_ }
        , core_database_{ name_ }
    {}

    void session::load()
    {
        core_database_.connect();
        window_system_.load(); // load before interface_system

        command_system_.load();
        context_system_.load();
        interface_system_.load();
    }

    void session::unload()
    {

    }

    const QString& session::id() const { return id_; }
    const QString& session::name() const { return name_; }
    bool session::is_active() const { return active_; }

    nxi::database& session::core_database() { return core_database_; }
    nxi::command_system& session::command_system() { return command_system_; }
    nxi::context_system& session::context_system() { return context_system_; }
    nxi::interface_system& session::interface_system() { return interface_system_; }
    nxi::window_system& session::window_system() { return window_system_; }
} // nxi