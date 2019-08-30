#include <ui/page/web.hpp>

#include <nxi/page/web.hpp>
#include <ui/renderer.hpp>
#include <ui/renderer/web.hpp>

#include <QWebEnginePage>
#include <QWebEngineSettings>

#include <nxi/config.hpp>
#include <nxi/core.hpp>
#include <ui/core.hpp>
#include <include/nxi/log.hpp>

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

        load(page_.command());

        connect(native_page_, &QWebEnginePage::urlChanged, this, [this](const QUrl& url)
        {
            qDebug() << "urlChanged" << url.toString();
            //page_.command_update(url.toString());
        });

        connect(native_page_, &QWebEnginePage::loadFinished, this, [this](bool n)
        {
            qDebug() << "COMPELTE " << n;
        });

        connect(native_page_, &QWebEnginePage::titleChanged, this, [this](const QString& name)
        {
            page_.name_update(name);
        });

        connect(native_page_, &QWebEnginePage::iconChanged, this, [this](const QIcon& icon)
        {
            emit page_.event_update_icon(icon);
        });



        connect(&page_, &nxi::web_page::event_load, this, [this]()
        {
            load(page_.command());
        });
    }

    void web_page::display(ui::renderer* renderer)
    {
        load(page_.command());
        renderer->display(this);
    }

    renderer* web_page::make_renderer() const
    {
        return new web_renderer;
    }

    void web_page::load(const QString& url)
    {
        nxi_trace("web_page::load {}", url);
        native_page_->load(QUrl::fromUserInput(url));
    }

    QWebEnginePage* web_page::native()
    {
        return native_page_;
    }
} // ui