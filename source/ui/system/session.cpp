#include <ui/system/session.hpp>

#include <nxi/core.hpp>
#include <nxi/system/session.hpp>
#include <nxi/system/window.hpp>
#include <ui/core.hpp>
#include <ui/system/window.hpp>

namespace ui
{
    session_system::session_system(ui::core& ui_core)
        : ui_core_{ ui_core }
    {
        connect(&ui_core_.nxi_core().session_system(), &nxi::session_system::event_add, [this](const nxi::session& session)
        {
            sessions_.emplace_back( std::make_unique<ui::session>(ui_core_, session.id()) );
        });

        connect(&ui_core_.nxi_core().session_system(), &nxi::session_system::event_focus_update, [this](const nxi::session& session)
        {
            focus_ = stz::make_observer(&get(session.id()));
        });
    }

    ui::session& session_system::focus() { return *focus_; }

    ui::session& session_system::get(const QString& session_id)
    {
        auto session_it = std::find_if(sessions_.begin(), sessions_.end(), [&session_id](auto&& s) { return s->id() == session_id; });
        nxi_assert(session_it != sessions_.end());
        return *session_it->get();
    }
} // ui
