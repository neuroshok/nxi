#ifndef INCLUDE_NXI_MODULE_JS_API_PAGE_SYSTEM_HPP_NXI
#define INCLUDE_NXI_MODULE_JS_API_PAGE_SYSTEM_HPP_NXI

namespace nxi { class core; }

#include <nxi/log.hpp>

#include <ui/page/web.hpp>

#include <QDebug>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QWebEnginePage>
#include <QVariantMap>

namespace nxi::js_api
{
    class page_system : public QObject
    {
        enum class event {add = 0, focus = 1};

        Q_OBJECT
    public:
        page_system(nxi::core& nxi_core, ui::web_page& ui_page)
            : nxi_core_{ nxi_core }
            , ui_page_{ ui_page }
        {}

        /*
        template<class Arg, class... Args>
        void call(const QString& function, const Arg& arg, const Args&... args)
        {
            QString str_args;
            if constexpr(sizeof...(Args) > 0) str_args = (... + ("," + args));
            QString script = function + "(" + arg + str_args + ");";
            nxi_trace("js run {}", script);
            ui_page_.native()->runJavaScript(script);
        }*/

    public slots:
        void add();
        void add_listener(int event_id, const QString& callback);

    signals:
        void js_callback(const QString&, QVariantMap);

    private:
        nxi::core& nxi_core_;
        ui::web_page& ui_page_;

        std::vector<QString> focus_callbacks_;
    };
} // nxi::js_api

#endif // INCLUDE_NXI_MODULE_JS_API_PAGE_SYSTEM_HPP_NXI
