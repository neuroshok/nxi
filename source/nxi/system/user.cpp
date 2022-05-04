#include <nxi/system/user.hpp>

#include <nxi/core.hpp>
#include <nxi/data/session.hpp>
#include <nxi/database.hpp>
#include <nxi/database/model.hpp>
#include <nxi/log.hpp>
#include <nxi/type.hpp>

#include <QDir>

namespace nxi
{
    user_system::user_system(nxi::core& nxi_core)
        : nxi_core_{ nxi_core }
    {}

    void user_system::load()
    {
        nxi_trace("");

        auto result = nxi::data::user::get(nxi_core_.global_database());
        while(result.next())
        {
            nxi::user_data data;
            data.id = result[nxi_model.user.id];
            data.name = result[nxi_model.user.name];
            data.active = result[nxi_model.user.active];

            users_.emplace_back(std::make_unique<nxi::user_session>(nxi_core_, std::move(data)) );

            auto& user = *users_.back();
            focus_ = stz::make_observer(&user);
            emit event_add(user);
            if (user.is_active()) load(user);
        }
        if (users_.size() == 0) add({ 0, "nxi", true});

        emit event_load();
    }

    void user_system::load(nxi::user_session& user)
    {
        nxi_trace("load user {}", user.name());
        focus(user);

        user.load();
        nxi::data::user::load(nxi_core_.global_database(), user.id());
    }

    void user_system::load(const QString& user_id)
    {
        load(get(user_id));
    }

    void user_system::add(nxi::user_data data)
    {
        nxi_trace("");
        nxi::data::user::add(nxi_core_.global_database(), data);
        users_.emplace_back( std::make_unique<nxi::user_session>(nxi_core_, std::move(data)) );
        auto& user = *users_.back();
        focus_ = stz::make_observer(&user);
        emit event_add(user);
        load(user);
    }

    void user_system::add(const QString& user_id)
    {
        add(nxi::user_data{ 0, user_id, true });
    }

    void user_system::del(const QString& user_id)
    {
        auto& user = get(user_id);
        unload(user_id);
        nxi::data::user::del(nxi_core_.global_database(), user.id());
        auto session_path = nxi::database::path + user_id;

        QDir dir(session_path);
        dir.removeRecursively();
    }

    void user_system::focus(nxi::user_session& user)
    {
        focus_ = stz::make_observer(&user);
        emit event_focus_update(*focus_);
    }

    void user_system::switch_focus(const QString& new_user_id)
    {
        focus(get(new_user_id));
    }

    nxi::user_session& user_system::get(int id)
    {
        auto session_it = std::find_if(users_.begin(), users_.end(), [id](auto&& s) { return s->id() == id; });
        nxi_assert(session_it != users_.end());
        return *session_it->get();
    }

    nxi::user_session& user_system::get(const QString& user_id)
    {
        auto session_it = std::find_if(users_.begin(), users_.end(), [&user_id](auto&& s) { return s->name() == user_id; });
        nxi_assert(session_it != users_.end());
        return *session_it->get();
    }
    stz::observer_ptr<nxi::user_session> user_system::focus()
    {
        nxi_assert(focus_ != nullptr);
        return focus_;
    }

    void user_system::unload()
    {
        nxi_trace("");
        for (auto& user : users_)
        {
            user->unload();
        }
    }

    void user_system::unload(const QString& user_id)
    {
        auto& user = get(user_id);
        user.unload();
        nxi::data::user::load(user.database(), user.id());
        emit event_unload(user);
    }
} // nxi