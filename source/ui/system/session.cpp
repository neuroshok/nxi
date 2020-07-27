#include <ui/system/session.hpp>

#include <nxi/core.hpp>
#include <nxi/system/session.hpp>
#include <nxi/system/window.hpp>

#include <ui/core.hpp>
#include <ui/interface/light/main.hpp>
#include <ui/system/window.hpp>

#include <QWebEngineProfile>
#include <QWebEngineSettings>

namespace ui
{
   session::session(ui::core& ui_core, nxi::session& nxi_session)
        : ui_core_{ ui_core }
        , nxi_core_{ ui_core_.nxi_core() }
        , nxi_session_{ nxi_session }
        , page_system_{ *this }
        , window_system_{ *this }
        , main_interface_{ [this](ui::window* window){ return new ui::interfaces::light::main(*this, window); } }
    {
        QWebEngineProfile::defaultProfile()->settings()->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled, true);
        QWebEngineProfile::defaultProfile()->settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);
        QWebEngineProfile::defaultProfile()->setHttpUserAgent("Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:10.0) Gecko/20100101 Firefox/74.0.1");
    }

    session::~session() { nxi_trace("ui::session {} deleted", id()); }

    const QString& session::id() const { return nxi_session_.id(); }

    nxi::core& session::nxi_core() { return nxi_core_; }
    ui::core& session::ui_core() { return ui_core_; }
    nxi::session& session::nxi_session() { return nxi_session_; }
    ui::page_system& session::page_system() { return page_system_; }
    ui::window_system& session::window_system() { return window_system_; }

    ui::main_interface* session::make_main_interface(ui::window* window) { return main_interface_(window); }
    void session::set_main_interface(std::function<ui::main_interface*(ui::window*)> fn) { main_interface_ = std::move(fn); }

    //

    session_system::session_system(ui::core& ui_core)
        : ui_core_{ ui_core }
    {
        connect(&ui_core_.nxi_core().session_system(), &nxi::session_system::event_add, [this](nxi::session& session)
        {
            sessions_.emplace_back( std::make_unique<ui::session>(ui_core_, session) );
        });

        connect(&ui_core_.nxi_core().session_system(), &nxi::session_system::event_unload, [this](nxi::session& session)
        {
            nxi_trace_event("");
            auto session_it = std::find_if(sessions_.begin(), sessions_.end(), [&session](auto&& s) { return s->id() == session.id(); });
            nxi_assert(session_it != sessions_.end());
            if (session_it != sessions_.end()) sessions_.erase(session_it);
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
