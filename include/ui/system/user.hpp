#ifndef INCLUDE_UI_SYSTEM_USER_HPP_NXI
#define INCLUDE_UI_SYSTEM_USER_HPP_NXI

#include <stz/observer_ptr.hpp>
#include <ui/system/page.hpp>
#include <ui/system/window.hpp>
#include <ui/user.hpp>
#include <functional>
#include <QObject>

namespace ui
{
    class core;

    class user_system : public QObject
    {
        Q_OBJECT
    public:
        user_system(ui::core&);

        ui::user& focus();
        ui::user& get(int session_id);

    private:
        ui::core& ui_core_;

        stz::observer_ptr<ui::user> focus_;
        std::vector<std::unique_ptr<ui::user>> sessions_;
    };
} // ui

#endif // INCLUDE_UI_SYSTEM_USER_HPP_NXI