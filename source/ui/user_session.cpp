#include <ui/user_session.hpp>

#include <nxi/core.hpp>
#include <nxi/system/user.hpp>
#include <nxi/system/window.hpp>
#include <nxi/user.hpp>

#include <ui/core.hpp>
#include <ui/interface/light/main.hpp>
#include <ui/system/window.hpp>

namespace ui
{
    user_session::user_session(ui::core& ui_core, nxi::user& nxi_user_session)
        : ui_core_{ ui_core }
        , nxi_core_{ ui_core_.nxi_core() }
        , nxi_session_{ nxi_user_session }
        , page_system_{ *this }
        , window_system_{ *this }
        , make_main_interface_{ [this](ui::window* window) { return new ui::interfaces::light::main(*this, window); } }
    {}

    user_session::~user_session() { nxi_trace("ui::user_session {} deleted", id()); }

    int user_session::id() const { return nxi_session_.id(); }

    ui::main_interface* user_session::make_main_interface(ui::window* window) { return make_main_interface_(window); }
    void user_session::set_main_interface(std::function<ui::main_interface*(ui::window*)> fn) { make_main_interface_ = std::move(fn); }

    nxi::core& user_session::nxi_core() { return nxi_core_; }
    ui::core& user_session::ui_core() { return ui_core_; }
    nxi::user& user_session::nxi_session() { return nxi_session_; }
    ui::page_system& user_session::page_system() { return page_system_; }
    ui::window_system& user_session::window_system() { return window_system_; }
} // ui