#ifndef INCLUDE_UI_USER_HPP_NXI
#define INCLUDE_UI_USER_HPP_NXI

#include <ui/system/page.hpp>
#include <ui/system/window.hpp>

#include <functional>

namespace nxi
{
    class core;
    class user;
} // nxi

namespace ui
{
    class core;
    class main_interface;
    class window;

    class user
    {
    public:
        user(ui::core& ui_core, nxi::user&);
        ~user();

        ui::main_interface* make_main_interface(ui::window*);
        void set_main_interface(std::function<ui::main_interface*(ui::window*)>);

        int id() const;

        nxi::core& nxi_core();
        nxi::user& nxi_user();
        ui::core& ui_core();
        ui::page_system& page_system();
        ui::window_system& window_system();

    private:
        ui::core& ui_core_;
        nxi::core& nxi_core_;
        nxi::user& nxi_user_;
        ui::page_system page_system_;
        ui::window_system window_system_;

        std::function<ui::main_interface*(ui::window*)> make_main_interface_;
    };
} //  nxi

#endif // INCLUDE_UI_USER_HPP_NXI