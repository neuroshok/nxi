#include <nxi/session.hpp>

#include <nxi/core.hpp>
#include <nxi/web_session.hpp>

namespace nxi
{
    session::session(nxi::core& core, nxi::session_data data)
        : core_{ core }
        , id_{ data.id }
        , name_{ std::move(data.name) }
        , active_{ std::move(data.active) }
        , config_{ "session", core_.user_database(), id_ }
        , web_session_{ new nxi::web_session{ *this, this } }
    {}

    void session::load()
    {
        nxi::data::session::load(core_.user_database(), id());
        web_session_->load();
    }

    void session::unload() { nxi::data::session::unload(core_.user_database(), id()); }

    int session::id() const { return id_; }
    const QString& session::name() const { return name_; }
    bool session::is_active() const { return active_; }

    const nxi::config& session::config() const { return config_; }
    nxi::config& session::config() { return config_; }

    nxi::core& session::core() { return core_; }
    nxi::web_session& session::web_session() { return *web_session_; }
} // nxi