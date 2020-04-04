#include <nxi/module/js_api/page_system.hpp>

#include <nxi/core.hpp>
#include <nxi/page/web.hpp>
#include <nxi/system/page.hpp>

struct tab
{

};

namespace nxi::js_api
{

    void page_system::add()
    {
        nxi_core_.page_system().open<nxi::web_page>();
    }

    void page_system::add_listener(int event_id, const QString& js_callback_source)
    {
        switch(event_id)
        {
            case event::add:

                break;
            case event::focus:
                focus_callbacks_.push_back(js_callback_source);
                connect(&nxi_core_.page_system(), &nxi::page_system::event_focus, this, [this](nds::node_ptr<nxi::page> page)
                {
                    QJsonObject activeInfo;
                    activeInfo["previousTabId"] = QJsonValue(0);
                    activeInfo["tabId"] = static_cast<int>(page->id());
                    activeInfo["windowId"] = QJsonValue(0);

                    QJsonDocument doc{ activeInfo };

                    for(const auto& callback : focus_callbacks_)
                    {
                        qDebug() << callback + "(" + doc.toJson(QJsonDocument::Compact) + ");";
                        ui_page_.native()->runJavaScript("(" + callback + ")(" + doc.toJson(QJsonDocument::Compact) + ");");
                    }
                });
                break;
            default:
                nxi_warning("unknown event {}", event_id);
        }
    }
} // nxi::js_api