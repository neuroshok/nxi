#include <nxi/system/session.hpp>

#include <nxi/core.hpp>
#include <nxi/data/session.hpp>
#include <nxi/database.hpp>
#include <nxi/database/model.hpp>
#include <nxi/log.hpp>
#include <nxi/type.hpp>

namespace nxi
{
    session_system::session_system(nxi::core& nxi_core)
        : nxi_core_{ nxi_core }
    {}

    void session_system::load()
    {
        nxi_trace("");

        auto result = nxi::data::session::get_sessions(nxi_core_.global_database());
        while(result.next())
        {
            nxi::session_data data;
            data.id = result[nxi_model.session.id];
            data.name = result[nxi_model.session.name];
            data.active = result[nxi_model.session.active];
            //sessions_.emplace_back( std::make_unique<nxi::user_session>(nxi_core_, std::move(data)) );

            auto& session = *sessions_.back();
            focus_ = stz::make_observer(&session);
            emit event_add(session);
            if (session.is_active()) load(session);
        }

        if (sessions_.size() == 0) add({ 0, "nxi", true });

        emit event_load();
    }

    void session_system::load(nxi::user_session& session)
    {
        nxi_trace("load session {}", session.name());
        focus(session);

        session.load();
        nxi::data::session::load_session(nxi_core_.global_database(), session.id());
    }

    void session_system::load(const QString& session_id)
    {
        load(get(session_id));
    }

    void session_system::add(nxi::session_data data)
    {
        nxi_trace("");
        nxi::data::session::add_session(nxi_core_.global_database(), data);
        //sessions_.emplace_back( std::make_unique<nxi::user_session>(nxi_core_, std::move(data)) );
        auto& session = *sessions_.back();
        focus_ = stz::make_observer(&session);
        emit event_add(session);
        load(session);
    }

    void session_system::add(const QString& session_id)
    {
        add(nxi::session_data{ 0, session_id, true });
    }

    void session_system::del(const QString& session_id)
    {
        auto& session = get(session_id);
        unload(session_id);
        nxi::data::session::del_session(nxi_core_.global_database(), session.id());
        auto session_path = nxi::database::path + session_id;

    }

    void session_system::focus(nxi::user_session& session)
    {
        focus_ = stz::make_observer(&session);
        emit event_focus_update(*focus_);
    }

    void session_system::switch_focus(const QString& new_session_id)
    {
        focus(get(new_session_id));
    }

    nxi::user_session& session_system::get(int id)
    {
        auto session_it = std::find_if(sessions_.begin(), sessions_.end(), [id](auto&& s) { return s->id() == id; });
        nxi_assert(session_it != sessions_.end());
        return *session_it->get();
    }

    nxi::user_session& session_system::get(const QString& session_id)
    {
        auto session_it = std::find_if(sessions_.begin(), sessions_.end(), [&session_id](auto&& s) { return s->name() == session_id; });
        nxi_assert(session_it != sessions_.end());
        return *session_it->get();
    }
    stz::observer_ptr<nxi::user_session> session_system::focus()
    {
        nxi_assert(focus_ != nullptr);
        return focus_;
    }

    void session_system::unload()
    {
        nxi_trace("");
        for (auto& session : sessions_)
        {
            session->unload();
        }
    }

    void session_system::unload(const QString& session_id)
    {
        auto& session = get(session_id);
        session.unload();
        nxi::data::session::load_session(session.database(), session.id());
        emit event_unload(session);
    }
} // nxi