#ifndef INCLUDE_NXI_USER_HPP_NXI
#define INCLUDE_NXI_USER_HPP_NXI

#include <nxi/data/user.hpp>
#include <nxi/database.hpp>
#include <nxi/module/api/core.hpp>

#include <nxi/system/command.hpp>
#include <nxi/system/context.hpp>
#include <nxi/system/interface.hpp>
#include <nxi/system/module.hpp>
#include <nxi/system/navigation.hpp>
#include <nxi/system/notification.hpp>
#include <nxi/system/page.hpp>
#include <nxi/system/session.hpp>
#include <nxi/system/window.hpp>

#include <QObject>
#include <QString>

namespace nxi
{
    struct config;
    class core;
    class web_session;

    class user : public QObject
    {
        Q_OBJECT

    public:
        user(nxi::core& core, nxi::user_data data);

        void load();
        void unload();

        void error(const QString& message) const;

        int id() const;
        const QString& name() const;
        bool is_active() const;

        nxi::api::core& api();
        nxi::config& config();
        nxi::database& database();
        nxi::core& nxi_core();

        nxi::command_system& command_system();
        nxi::context_system& context_system();
        nxi::interface_system& interface_system();
        nxi::module_system& module_system();
        nxi::navigation_system& navigation_system();
        nxi::notification_system& notification_system();
        nxi::page_system& page_system();
        nxi::session_system& session_system();
        nxi::window_system& window_system();

    signals:
        void event_error(const QString&) const;

    private:
        nxi::core& nxi_core_;

        int id_;
        QString name_;
        bool active_;

        nxi::core_database user_database_;

        nxi::api::core api_;
        std::unique_ptr<nxi::config> config_;

        nxi::session_system session_system_;
        nxi::command_system command_system_;
        nxi::context_system context_system_;
        nxi::interface_system interface_system_;
        nxi::page_system page_system_;
        nxi::window_system window_system_;
        nxi::module_system module_system_;
        nxi::navigation_system navigation_system_;
        nxi::notification_system notification_system_;
    };
} // nxi

#endif // INCLUDE_NXI_USER_HPP_NXI