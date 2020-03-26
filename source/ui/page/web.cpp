#include <ui/page/web.hpp>

#include <nxi/page/web.hpp>
#include <ui/renderer.hpp>
#include <ui/renderer/web.hpp>

#include <QWebEngineFullScreenRequest>
#include <QWebEnginePage>
#include <QWebEngineSettings>


#include <nxi/config.hpp>
#include <nxi/core.hpp>
#include <ui/core.hpp>
#include <include/nxi/log.hpp>
#include <ui/window.hpp>
#include <ui/interface/main.hpp>

namespace ui
{
    web_page::web_page(ui::core& ui_core, nxi::web_page& page) :
        ui::page{ page }
        , page_{ page }
        , ui_core_{ ui_core }
    {
        native_page_ = new QWebEnginePage(this);
        native_page_->settings()->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled, true);
        native_page_->settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);

        connect(native_page_, &QWebEnginePage::fullScreenRequested, [this](QWebEngineFullScreenRequest request)
        {
            static_cast<ui::window*>(native_page_->view()->window())->main_interface()->toggle_fullmode();
            request.accept();
        });

        connect(native_page_, &QWebEnginePage::urlChanged, this, [this](const QUrl& url)
        {
            nxi_debug("{}",  url.toString());
            page_.update_command(url.toString());
        });

        connect(native_page_, &QWebEnginePage::loadFinished, this, [this](bool n)
        {
            nxi_debug("load complete");
        });

        connect(native_page_, &QWebEnginePage::titleChanged, this, [this](const QString& name)
        {
            page_.update_name(name);
        });

        connect(native_page_, &QWebEnginePage::iconChanged, this, [this](const QIcon& icon)
        {
            page_.update_icon(icon);
        });


        connect(&page_, &nxi::web_page::event_load, this, [this]()
        {
            qDebug() << "dbg event_load";
            load(page_.command());
        });
    }

    void web_page::display(ui::renderer* renderer)
    {
        renderer->display(this);
    }

    ui::renderer* web_page::make_renderer() const
    {
        return new web_renderer;
    }

    void web_page::load(const QString& url)
    {
        nxi_trace("{}", url);
        native_page_->load(QUrl::fromUserInput(url));
    }

    QWebEnginePage* web_page::native()
    {
        return native_page_;
    }
} // ui