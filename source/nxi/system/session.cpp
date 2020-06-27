#include <nxi/system/session.hpp>

#include <nxi/core.hpp>
#include <nxi/database.hpp>
#include <nxi/database/model.hpp>
#include <nxi/log.hpp>

namespace nxi
{
    session_system::session_system(nxi::core& nxi_core)
        : nxi_core_{ nxi_core }
    {}

    void session_system::load()
    {
        nxi_trace("");

        auto result = nxi::data::session::get_sessions(nxi_core_);
        while(result.next())
        {
            nxi::session_data data;
            data.name = result[nxi_model.session.name];
            data.active = result[nxi_model.session.active];
            sessions_.emplace_back( std::make_unique<nxi::session>(nxi_core_, std::move(data)) );

            auto& session = *sessions_.back();
            focus_ = stz::make_observer(&session);
            emit event_add(session);
            if (session.is_active()) load(session);
        }
        if (sessions_.size() == 0) add({ "default", true });
    }

    void session_system::load(nxi::session& session)
    {
        nxi_trace("load session {}", session.name());
        focus(session);

        session.load();
/*
        nxi_core_.command_system().load();
        nxi_core_.page_system().load();
        nxi_core_.interface_system().load();
        nxi_core_.context_system().load();

        nxi_core_.module_system().load();*/
    }

    void session_system::load(const QString& session_id)
    {
        load(get(session_id));
    }

    void session_system::add(nxi::session_data data)
    {
        nxi_trace("");
        nxi::data::session::add_session(nxi_core_, data.name);
        sessions_.emplace_back( std::make_unique<nxi::session>(nxi_core_, std::move(data)) );
        auto& session = *sessions_.back();
        focus_ = stz::make_observer(&session);
        emit event_add(session);
        load(session);
    }

    void session_system::add(const QString& session_id)
    {
        add(nxi::session_data{ session_id, true });
    }

    void session_system::focus(nxi::session& session)
    {
        focus_ = stz::make_observer(&session);
        emit event_focus_update(*focus_);
    }

    void session_system::switch_focus(const QString& new_session_id)
    {
        if (focus_->name() != new_session_id)
        {
            load(new_session_id);
            focus_->unload();
            //focus_->load();
        }
    }

    nxi::session& session_system::get(const QString& session_id)
    {
        auto session_it = std::find_if(sessions_.begin(), sessions_.end(), [&session_id](auto&& s) { return s->name() == session_id; });
        nxi_assert(session_it != sessions_.end());
        return *session_it->get();
    }
    stz::observer_ptr<nxi::session> session_system::focus()
    {
        nxi_assert(focus_ != nullptr);
        return focus_;
    }
} // nxi
