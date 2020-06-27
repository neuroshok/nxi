#ifndef INCLUDE_UI_SYSTEM_SESSION_HPP_NXI
#define INCLUDE_UI_SYSTEM_SESSION_HPP_NXI

#include <ui/system/window.hpp>
#include <stz/observer_ptr.hpp>
#include <QObject>

namespace ui
{
    class core;

    class session
    {
    public:
        session(ui::core& ui_core, QString id)
            : ui_core_{ ui_core }
            , id_{ std::move(id) }
            , window_system_{ ui_core_ }
        {}

        const QString& id() const { return id_; }
        ui::window_system& window_system() { return window_system_; }

    private:
        ui::core& ui_core_;
        QString id_;
        ui::window_system window_system_;
    };

    class session_system : public QObject
    {
        Q_OBJECT
    public:
        session_system(ui::core& ui_core);

        ui::session& focus();
        ui::session& get(const QString& session_id);

    private:
        ui::core& ui_core_;

        stz::observer_ptr<ui::session> focus_;
        std::vector<std::unique_ptr<ui::session>> sessions_;
    };
} // ui

#endif // INCLUDE_UI_SYSTEM_SESSION_HPP_NXI
