#ifndef INCLUDE_UI_PAGE_WEB_ENGINE_HPP_NXI
#define INCLUDE_UI_PAGE_WEB_ENGINE_HPP_NXI

#include <QWebEnginePage>

class QWebEngineProfile;

namespace ui
{
    class web_engine_page : public QWebEnginePage
    {
    public:
        web_engine_page(QWebEngineProfile*, QObject* parent = nullptr);

        virtual bool acceptNavigationRequest(const QUrl& url, QWebEnginePage::NavigationType type, bool is_mainframe) override;

    protected:
        void javaScriptConsoleMessage(QWebEnginePage::JavaScriptConsoleMessageLevel
            , const QString& message, int line, const QString& sourceID) override;

    };
} // ui

#endif // INCLUDE_UI_PAGE_WEB_ENGINE_HPP_NXI