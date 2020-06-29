#ifndef INCLUDE_NXI_SESSION_HPP_NXI
#define INCLUDE_NXI_SESSION_HPP_NXI

#include <nxi/database.hpp>
#include <nxi/data/session.hpp>

#include <nxi/system/command.hpp>
#include <nxi/system/context.hpp>
#include <nxi/system/interface.hpp>
#include <nxi/system/module.hpp>
#include <nxi/system/page.hpp>
#include <nxi/system/window.hpp>

#include <nxi/module/api/core.hpp>

#include <array>
#include <vector>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

namespace nxi
{
    class core;

    class session : public QObject
    {
        Q_OBJECT

    public:
        session(nxi::core& core, nxi::session_data data);

        void load();
        void unload();

        void error(const QString& message) const;

        const QString& id() const;
        const QString& name() const;
        bool is_active() const;

        nxi::api::core& api();
        nxi::core& nxi_core();

        nxi::database& database();
        nxi::command_system& command_system();
        nxi::context_system& context_system();
        nxi::interface_system& interface_system();
        nxi::module_system& module_system();
        nxi::page_system& page_system();
        nxi::window_system& window_system();

    signals:
        void event_error(const QString&) const;

    private:
        nxi::core& nxi_core_;

        QString id_;
        QString name_;
        bool active_;

        nxi::api::core api_;

        nxi::command_system command_system_;
        nxi::context_system context_system_;
        nxi::interface_system interface_system_;
        nxi::page_system page_system_;
        nxi::window_system window_system_;
        nxi::module_system module_system_;

        nxi::core_database core_database_;
    };
} // nxi

#endif // INCLUDE_NXI_SESSION_HPP_NXI
