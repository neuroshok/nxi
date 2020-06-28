#ifndef INCLUDE_NXI_SESSION_HPP_NXI
#define INCLUDE_NXI_SESSION_HPP_NXI

#include <nxi/database.hpp>
#include <nxi/data/session.hpp>

#include <nxi/system/command.hpp>
#include <nxi/system/context.hpp>
#include <nxi/system/interface.hpp>
#include <nxi/system/window.hpp>

#include <array>
#include <vector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

namespace nxi
{
    class core;

    class session
    {
    public:
        session(nxi::core& core, nxi::session_data data);

        void load();

        void unload();

        const QString& id() const;
        const QString& name() const;
        bool is_active() const;

        nxi::database& core_database();
        nxi::command_system& command_system();
        nxi::context_system& context_system();
        nxi::interface_system& interface_system();
        nxi::window_system& window_system();

    private:
        nxi::core& nxi_core_;

        QString id_;
        QString name_;
        bool active_;

        nxi::command_system command_system_;
        nxi::context_system context_system_;
        nxi::interface_system interface_system_;
        nxi::window_system window_system_;
        nxi::core_database core_database_;
    };
} // nxi

#endif // INCLUDE_NXI_SESSION_HPP_NXI
