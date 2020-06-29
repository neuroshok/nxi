#include <nxi/module/api/page_system.hpp>
#include <nxi/module/api/js.hpp>

#include <nxi/core.hpp>
#include <nxi/page/web.hpp>
#include <nxi/system/page.hpp>

struct tab
{

};

namespace nxi::api
{

    void page_system::add()
    {
        session_.page_system().open<nxi::web_page>();
    }

    void page_system::add_listener(int event_id, const QString& js_callback_source)
    {
        switch(static_cast<event>(event_id))
        {
            case event::add:

                break;
            case event::focus:
                focus_callbacks_.push_back(js_callback_source);
                connect(&session_.page_system(), &nxi::page_system::event_focus, this, [this](nds::node_ptr<nxi::page> page)
                {
                    QJsonObject activeInfo;
                    activeInfo["previousTabId"] = 0;
                    activeInfo["tabId"] = static_cast<int>(page->id());
                    activeInfo["windowId"] = 0;

                    QJsonDocument doc{ activeInfo };

                    for(const auto& callback : focus_callbacks_)
                    {
                        nxi::api::js_call(*page, callback, activeInfo);
                    }
                });
                break;
            default:
                nxi_warning("unknown event {}", event_id);
        }
    }
} // nxi::api