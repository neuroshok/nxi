#ifndef INCLUDE_UI_SYSTEM_SESSION_HPP_NXI
#define INCLUDE_UI_SYSTEM_SESSION_HPP_NXI

#include <ui/system/page.hpp>
#include <ui/system/window.hpp>
#include <stz/observer_ptr.hpp>
#include <functional>
#include <QObject>

namespace nxi
{
    class core;
    class session;
} // nxi

namespace ui
{
    class core;
    class main_interface;
    class window;

    class session
    {
    public:
        session(ui::core& ui_core, nxi::session&);
        ~session();

        int id() const;

        nxi::core& nxi_core();
        nxi::session& nxi_session();
        ui::core& ui_core();
        ui::page_system& page_system();
        ui::window_system& window_system();

        ui::main_interface* make_main_interface(ui::window*);
        void set_main_interface(std::function<ui::main_interface*(ui::window*)>);

    private:
        ui::core& ui_core_;
        nxi::core& nxi_core_;
        nxi::session& nxi_session_;
        ui::page_system page_system_;
        ui::window_system window_system_;

        std::function<ui::main_interface*(ui::window*)> main_interface_;
    };

    class session_system : public QObject
    {
        Q_OBJECT
    public:
        session_system(ui::core&);

        ui::session& focus();
        ui::session& get(int session_id);

    private:
        ui::core& ui_core_;

        stz::observer_ptr<ui::session> focus_;
        std::vector<std::unique_ptr<ui::session>> sessions_;
    };
} // ui

#endif // INCLUDE_UI_SYSTEM_SESSION_HPP_NXI