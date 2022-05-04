#ifndef INCLUDE_UI_SYSTEM_USER_HPP_NXI
#define INCLUDE_UI_SYSTEM_USER_HPP_NXI

#include <ui/user_session.hpp>
#include <ui/system/page.hpp>
#include <ui/system/window.hpp>
#include <stz/observer_ptr.hpp>
#include <functional>
#include <QObject>

namespace nxi
{
    class core;
    class user_session;
} // nxi

namespace ui
{
    class core;

    class user_system : public QObject
    {
        Q_OBJECT
    public:
        user_system(ui::core&);

        ui::user_session& focus();
        ui::user_session& get(int session_id);

    private:
        ui::core& ui_core_;

        stz::observer_ptr<ui::user_session> focus_;
        std::vector<std::unique_ptr<ui::user_session>> sessions_;
    };
} // ui

#endif // INCLUDE_UI_SYSTEM_USER_HPP_NXI