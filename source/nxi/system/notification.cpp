#include <nxi/system/notification.hpp>

#include <nxi/notification_data.hpp>

#include <QWebEngineNotification>

namespace nxi
{
    notification_system::notification_system(nxi::core& core)
        : core_{ core }
    {}

    void notification_system::send(const QString& message)
    {
        nxi::notification_data notification;
        notification.type = notification_type::system;
        notification.message = message;
        send(std::move(notification));
    }

    void notification_system::send(const nxi::notification_data& notification) { emit event_send(notification); }

    void notification_system::send(const QWebEngineNotification& web_notification)
    {
        nxi::notification_data notification;
        notification.type = notification_type::web;
        notification.source = web_notification.origin().toString();
        notification.title = web_notification.title();
        notification.message = web_notification.message();
        send(std::move(notification));
    }
} // nxi