#ifndef INCLUDE_NXI_SYSTEM_NOTIFICATION_HPP_NXI
#define INCLUDE_NXI_SYSTEM_NOTIFICATION_HPP_NXI

#include <QObject>

class QString;
class QWebEngineNotification;

namespace nxi
{
    class core;
    struct notification_data;

    class notification_system : public QObject
    {
        Q_OBJECT
    public:
        explicit notification_system(nxi::core&);
        notification_system(const notification_system&) = delete;
        notification_system& operator=(const notification_system&) = delete;

        void send(const QString& message);
        void send(const nxi::notification_data&);
        void send(const QWebEngineNotification&);

    signals:
        void event_send(const nxi::notification_data&);

    private:
        nxi::core& core_;
    };
} // nxi

#endif // INCLUDE_NXI_SYSTEM_NOTIFICATION_HPP_NXI