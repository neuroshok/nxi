#ifndef INCLUDE_NXI_MODULE_API_PAGE_SYSTEM_HPP_NXI
#define INCLUDE_NXI_MODULE_API_PAGE_SYSTEM_HPP_NXI

namespace nxi { class core; }

#include <nxi/log.hpp>

#include <ui/page/web.hpp>

#include <QDebug>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QWebEnginePage>
#include <QVariantMap>

namespace nxi::api
{
    class page_system : public QObject
    {
        enum class event {add = 0, focus = 1};

        Q_OBJECT
    public:
        page_system(nxi::core& nxi_core)
            : nxi_core_{ nxi_core }
        {}

    public slots:
        void add();
        void add_listener(int event_id, const QString& callback);

    signals:
        void js_callback(const QString&, QVariantMap);

    private:
        nxi::core& nxi_core_;

        std::vector<QString> focus_callbacks_;
    };
} // nxi::api

#endif // INCLUDE_NXI_MODULE_API_PAGE_SYSTEM_HPP_NXI
