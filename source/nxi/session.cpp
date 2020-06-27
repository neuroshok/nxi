#include <nxi/session.hpp>

namespace nxi
{
    session::session(nxi::core& core, nxi::session_data data)
        : nxi_core_{ core }
        , name_{ std::move(data.name) }
        , active_{ std::move(data.active) }
        , window_system_{ nxi_core_ }
        , core_database_{ name_ }
    {}

    void session::load()
    {
        core_database_.connect();
        window_system_.load();
    }

    void session::unload()
    {

    }

    const QString& session::id() const { return id_; }
    const QString& session::name() const { return name_; }
    bool session::is_active() const { return active_; }

    nxi::database& session::core_database() { return core_database_; }
    nxi::window_system& session::window_system() { return window_system_; }
} // nxi