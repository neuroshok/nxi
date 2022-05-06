#include <ui/page/web.hpp>

#include <nxi/config.hpp>
#include <nxi/core.hpp>
#include <nxi/log.hpp>
#include <nxi/module/api/core.hpp>
#include <nxi/module/api/page_system.hpp>
#include <nxi/page/web.hpp>
#include <nxi/user.hpp>
#include <nxi/web_session.hpp>

#include <ui/core.hpp>
#include <ui/interface/main.hpp>
#include <ui/page/web_engine.hpp>
#include <ui/renderer.hpp>
#include <ui/renderer/web.hpp>
#include <ui/window.hpp>

#include <QWebEngineFullScreenRequest>
#include <QWebEngineProfile>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>
#include <QWebEngineSettings>
#include <QWebEngineView>

namespace ui
{
    web_page::web_page(ui::user_session& session, nxi::web_page& page)
        : ui::page{ page }
        , page_{ page }
        , session_{ session }
    {
        auto& websession = session_.nxi_core().session_system().get(page_.session_id()).web_session();
        native_page_ = new ui::web_engine_page(static_cast<QWebEngineProfile*>(&websession), this);
        native_page_->setWebChannel(session_.nxi_session().module_system().web_channel());

        native_page_->setAudioMuted(page.is_muted());

        connect(&page, &nxi::web_page::event_add_script, [this](const QWebEngineScript& script) { native_page_->scripts().insert(script); });
        connect(native_page_, &QWebEnginePage::fullScreenRequested, [this](QWebEngineFullScreenRequest request) {
            // return state
            auto interface = static_cast<ui::window*>(QWebEngineView::forPage(native_page_)->window())->main_interface();
            bool accepted = interface->toggle_fullmode();
            request.accept();
            // qDebug() << "MODE " << interface->fullmode() ;
            // native_page_->setFullScreenMode(interface->fullmode());
        });
        connect(native_page_, &QWebEnginePage::featurePermissionRequested, [this](const QUrl& origin, QWebEnginePage::Feature feature) {
            native_page_->setFeaturePermission(origin, feature, QWebEnginePage::PermissionPolicy::PermissionGrantedByUser);
        });
        connect(native_page_, &QWebEnginePage::loadStarted, this, [&websession, this]() {
            websession.load_cookie(native_page_->requestedUrl().host(), page_.session_id());
        });
        connect(native_page_, &QWebEnginePage::loadFinished, this, [this](bool n) {
            // ui_core_.module_system().process(page_);
        });
        connect(native_page_, &QWebEnginePage::iconChanged, this, [this](const QIcon& icon) { page_.update_icon(icon); });
        connect(native_page_, &QWebEnginePage::titleChanged, this, [this](const QString& name) { page_.update_name(name); });
        connect(native_page_, &QWebEnginePage::recentlyAudibleChanged, this, [this](bool state) { page_.update_audible(state); });
        connect(native_page_, &QWebEnginePage::urlChanged, this, [this](const QUrl& url) { page_.update_command(url.toString()); });

        //

        connect(&page_, &nxi::web_page::event_load, this, [this]() { load(page_.command()); });
        connect(&page_, &nxi::web_page::event_run_script, this, [this](const QString& script) { native_page_->runJavaScript(script); });
        connect(&page_, &nxi::web_page::event_update_mute, this, [this](bool state) { native_page_->setAudioMuted(state); });
        connect(&page_, &nxi::web_page::event_call_script, this, [this](const QString& script, std::function<void(const QVariant&)> fn) {
            native_page_->runJavaScript(script, [f = std::move(fn)](const QVariant& result) {
                if (f) f(result);
            });
        });

        session_.nxi_session().module_system().process(page_);
    }

    void web_page::display(ui::renderer* renderer) { renderer->display(this); }

    ui::renderer* web_page::make_renderer() const { return new web_renderer; }

    void web_page::load(const QString& url)
    {
        nxi_trace("{}", url);
        native_page_->load(QUrl::fromUserInput(url));
    }

    QWebEnginePage* web_page::native() { return static_cast<QWebEnginePage*>(native_page_); }
} // ui