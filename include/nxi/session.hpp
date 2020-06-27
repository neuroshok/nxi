#ifndef INCLUDE_NXI_SESSION_HPP_NXI
#define INCLUDE_NXI_SESSION_HPP_NXI

#include <nxi/database.hpp>
#include <nxi/data/session.hpp>

#include <include/nxi/system/window.hpp>
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
        nxi::window_system& window_system();

    private:
        nxi::core& nxi_core_;

        QString id_;
        QString name_;
        bool active_;

        nxi::window_system window_system_;
        nxi::core_database core_database_;
    };
} // nxi

#endif // INCLUDE_NXI_SESSION_HPP_NXI
