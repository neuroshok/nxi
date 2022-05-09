#ifndef INCLUDE_NXI_MODULE_API_PAGE_SYSTEM_HPP_NXI
#define INCLUDE_NXI_MODULE_API_PAGE_SYSTEM_HPP_NXI

#include <nxi/log.hpp>

#include <QDebug>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QVariantMap>
#include <QWebEnginePage>

namespace nxi
{
    class core;
}

namespace nxi::api
{
    class page_system : public QObject
    {
        enum class event
        {
            add = 0,
            focus = 1
        };

        Q_OBJECT
    public:
        page_system(nxi::core& core)
            : core_{ core }
        {}
        page_system(const page_system&) = delete;
        page_system& operator=(const page_system&) = delete;

    public slots:
        void add();
        void add_listener(int event_id, const QString& callback);

    signals:
        void js_callback(const QString&, QVariantMap);

    private:
        nxi::core& core_;

        std::vector<QString> focus_callbacks_;
    };
} // nxi::api

#endif // INCLUDE_NXI_MODULE_API_PAGE_SYSTEM_HPP_NXI