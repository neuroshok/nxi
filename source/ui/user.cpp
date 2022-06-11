#include <ui/user.hpp>

#include <nxi/core.hpp>
#include <nxi/system/user.hpp>
#include <nxi/system/window.hpp>
#include <nxi/user.hpp>

#include <ui/core.hpp>
#include <ui/interface/light/main.hpp>
#include <ui/system/window.hpp>

namespace ui
{
    user::user(ui::core& ui_core, nxi::user& nxi_user)
        : ui_core_{ ui_core }
        , nxi_core_{ ui_core_.nxi_core() }
        , nxi_user_{ nxi_user }
        , page_system_{ *this }
        , window_system_{ *this }
        , make_main_interface_{ [this](ui::window* window) { return new ui::interfaces::light::main(*this, window); } }
    {}

    user::~user() { nxi_trace("ui::user {} deleted", id()); }

    int user::id() const { return nxi_user_.id(); }

    ui::main_interface* user::make_main_interface(ui::window* window) { return make_main_interface_(window); }
    void user::set_main_interface(std::function<ui::main_interface*(ui::window*)> fn) { make_main_interface_ = std::move(fn); }

    nxi::core& user::nxi_core() { return nxi_core_; }
    ui::core& user::ui_core() { return ui_core_; }
    nxi::user& user::nxi_user() { return nxi_user_; }
    ui::page_system& user::page_system() { return page_system_; }
    ui::window_system& user::window_system() { return window_system_; }
} // ui