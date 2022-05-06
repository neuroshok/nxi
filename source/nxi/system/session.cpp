#include <nxi/system/session.hpp>

#include <nxi/core.hpp>
#include <nxi/data/session.hpp>
#include <nxi/database.hpp>
#include <nxi/database/model.hpp>
#include <nxi/log.hpp>
#include <nxi/session.hpp>
#include <nxi/type.hpp>
#include <nxi/user.hpp>

namespace nxi
{
    session_system::session_system(nxi::core& core)
        : core_{ core }
    {}

    void session_system::load()
    {
        nxi_trace("");

        auto result = nxi::data::session::get(core_.user_database());
        while (result.next())
        {
            nxi::session_data data;
            data.id = result[nxi_model.session.id];
            data.name = result[nxi_model.session.name];
            data.active = result[nxi_model.session.active];
            sessions_.emplace_back(std::make_unique<nxi::session>(core_, std::move(data)));

            auto& session = *sessions_.back();
            focus_ = stz::make_observer(&session);
            emit event_add(session);
            if (session.is_active()) load(session);

            focus(session);
        }

        if (sessions_.size() == 0) add({ 0, "nxi", true });

        emit event_load();
    }

    void session_system::load(nxi::session& session)
    {
        nxi_trace("load session {}", session.name());
        session.load();
        focus(session);
    }

    void session_system::load(const QString& session_id) { load(get(session_id)); }

    void session_system::add(nxi::session_data data)
    {
        nxi_trace("");
        auto session_id = nxi::data::session::add(core_.user_database(), data);
        data.id = session_id;
        sessions_.emplace_back(std::make_unique<nxi::session>(core_, std::move(data)));
        auto& session = *sessions_.back();
        focus_ = stz::make_observer(&session);
        emit event_add(session);
        load(session);
    }

    void session_system::add(const QString& session_id) { add(nxi::session_data{ 0, session_id, true }); }

    void session_system::del(int session_id)
    {
        if (sessions_.size() == 1)
        {
            nxi_warning("Can't delete last session");
            return;
        }
        // if (!exist(session_name))
        nxi::data::session::del(core_.user_database(), session_id);
        sessions_.erase(std::remove_if(sessions_.begin(), sessions_.end(), [session_id](const auto& session) { return session_id == session->id(); }),
                        sessions_.end());

        focus(*sessions_.back());
    }

    void session_system::focus(nxi::session& session)
    {
        focus_ = stz::make_observer(&session);
        emit event_focus(*focus_);
    }

    void session_system::switch_focus(const QString& new_session_id) { focus(get(new_session_id)); }

    nxi::session& session_system::get(int id)
    {
        // todo default session id, should return the focused session ?
        if (id == 0)
        {
            nxi_assert(!sessions_.empty());
            return *sessions_[0];
        }
        auto session_it = std::find_if(sessions_.begin(), sessions_.end(), [id](auto&& s) { return s->id() == id; });
        nxi_assert(session_it != sessions_.end());
        return *session_it->get();
    }

    nxi::session& session_system::get(const QString& session_id)
    {
        auto session_it = std::find_if(sessions_.begin(), sessions_.end(), [&session_id](auto&& s) { return s->name() == session_id; });
        nxi_assert(session_it != sessions_.end());
        return *session_it->get();
    }

    nxi::session& session_system::focus()
    {
        nxi_assert(focus_ != nullptr);
        return *focus_;
    }

    void session_system::unload()
    {
        nxi_trace("");
        for (auto& session : sessions_)
        {
            session->unload();
        }
    }

    void session_system::unload(const QString& session_id) {}

    const std::vector<std::unique_ptr<nxi::session>>& session_system::sessions() const { return sessions_; }
} // nxi