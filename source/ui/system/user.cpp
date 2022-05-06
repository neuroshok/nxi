#include <ui/system/user.hpp>

#include <nxi/core.hpp>
#include <nxi/system/session.hpp>
#include <nxi/system/window.hpp>
#include <nxi/user.hpp>

#include <ui/core.hpp>
#include <ui/interface/light/main.hpp>
#include <ui/system/window.hpp>

namespace ui
{
    user_system::user_system(ui::core& ui_core)
        : ui_core_{ ui_core }
    {
        connect(&ui_core_.nxi_core().user_system(), &nxi::user_system::event_add, [this](nxi::user& user) {
            sessions_.emplace_back(std::make_unique<ui::user_session>(ui_core_, user));
        });

        connect(&ui_core_.nxi_core().user_system(), &nxi::user_system::event_unload, [this](nxi::user& user) {
            nxi_trace_event("");
            auto session_it = std::find_if(sessions_.begin(), sessions_.end(), [&user](auto&& s) { return s->id() == user.id(); });
            nxi_assert(session_it != sessions_.end());
            if (session_it != sessions_.end()) sessions_.erase(session_it);
        });

        connect(&ui_core_.nxi_core().user_system(), &nxi::user_system::event_focus, [this](const nxi::user& user) {
            focus_ = stz::make_observer(&get(user.id()));
        });
    }

    ui::user_session& user_system::focus() { return *focus_; }

    ui::user_session& user_system::get(int session_id)
    {
        auto session_it = std::find_if(sessions_.begin(), sessions_.end(), [&session_id](auto&& s) { return s->id() == session_id; });
        nxi_assert(session_it != sessions_.end());
        return *session_it->get();
    }
} // ui