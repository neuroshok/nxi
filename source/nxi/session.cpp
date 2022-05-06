#include <nxi/session.hpp>

#include <nxi/user_session.hpp>
#include <nxi/web_session.hpp>

namespace nxi
{
    session::session(nxi::user_session& user_session, nxi::session_data data)
        : user_session_{ user_session }
        , id_{ data.id }
        , name_{ std::move(data.name) }
        , active_{ std::move(data.active) }
        , config_{ "session", user_session_.database(), id_ }
        , web_session_{ new nxi::web_session{ *this, this } }
    {}

    void session::load()
    {
        nxi::data::session::load(user_session_.database(), id());
        web_session_->load();
    }

    void session::unload()
    {
        nxi::data::session::unload(user_session_.database(), id());
    }

    int session::id() const { return id_; }
    const QString& session::name() const { return name_; }
    bool session::is_active() const { return active_; }

    const nxi::config& session::config() const { return config_; }
    nxi::config& session::config() { return config_; }

    nxi::user_session& session::user_session() { return user_session_; }
    nxi::web_session& session::web_session() { return *web_session_; }
} // nxi