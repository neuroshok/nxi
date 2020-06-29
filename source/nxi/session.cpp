#include <nxi/session.hpp>

namespace nxi
{
    session::session(nxi::core& core, nxi::session_data data)
        : nxi_core_{ core }
        , id_{ data.name }
        , name_{ std::move(data.name) }
        , active_{ std::move(data.active) }
        , api_{ *this }
        , command_system_{ *this }
        , context_system_{ *this }
        , interface_system_{ *this }
        , page_system_{ *this }
        , window_system_{ *this }
        , module_system_{ *this }
        , core_database_{ name_ }
    {}

    void session::load()
    {
        core_database_.connect();
        window_system_.load(); // load before interface_system

        command_system_.load();
        context_system_.load();
        interface_system_.load();
        page_system_.load();
        module_system_.load();
    }

    void session::unload()
    {

    }

    void session::error(const QString& message) const
    {
        emit event_error(message);
    }

    const QString& session::id() const { return id_; }
    const QString& session::name() const { return name_; }
    bool session::is_active() const { return active_; }

    nxi::core& session::nxi_core() { return nxi_core_; }
    nxi::api::core& session::api() { return api_; }
    nxi::database& session::database() { return core_database_; }

    nxi::command_system& session::command_system() { return command_system_; }
    nxi::context_system& session::context_system() { return context_system_; }
    nxi::interface_system& session::interface_system() { return interface_system_; }
    nxi::module_system& session::module_system() { return module_system_; }
    nxi::page_system& session::page_system() { return page_system_; }
    nxi::window_system& session::window_system() { return window_system_; }
} // nxi