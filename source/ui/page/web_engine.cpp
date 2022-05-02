#include <ui/page/web_engine.hpp>

#include <nxi/log.hpp>

namespace ui
{
    web_engine_page::web_engine_page(QWebEngineProfile* profile, QObject* parent)
        : QWebEnginePage(profile, parent)
    {}

    void web_engine_page::javaScriptConsoleMessage(QWebEnginePage::JavaScriptConsoleMessageLevel level, const QString& message,
                                              int line, const QString& sourceID)
    {
        if (level == JavaScriptConsoleMessageLevel::ErrorMessageLevel) nxi_error("[JS] {} - {}", line, message);
    }

    bool web_engine_page::acceptNavigationRequest(const QUrl& url, QWebEnginePage::NavigationType type, bool is_mainframe)
    {
        return QWebEnginePage::acceptNavigationRequest(url, type, is_mainframe);

    }
} // ui