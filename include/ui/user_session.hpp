#ifndef INCLUDE_UI_USER_SESSION_HPP_NXI
#define INCLUDE_UI_USER_SESSION_HPP_NXI

#include <ui/system/page.hpp>
#include <ui/system/window.hpp>

#include <functional>

namespace nxi
{
    class core;
    class user_session;
} // nxi

namespace ui
{
    class core;
    class main_interface;
    class window;

    class user_session
    {
    public:
        user_session(ui::core& ui_core, nxi::user_session&);
        ~user_session();

        int id() const;

        nxi::core& nxi_core();
        nxi::user_session& nxi_session();
        ui::core& ui_core();
        ui::page_system& page_system();
        ui::window_system& window_system();

        ui::main_interface* make_main_interface(ui::window*);
        void set_main_interface(std::function<ui::main_interface*(ui::window*)>);

    private:
        ui::core& ui_core_;
        nxi::core& nxi_core_;
        nxi::user_session& nxi_session_;
        ui::page_system page_system_;
        ui::window_system window_system_;

        std::function<ui::main_interface*(ui::window*)> main_interface_;
    };
} //  nxi

#endif // INCLUDE_UI_USER_SESSION_HPP_NXI