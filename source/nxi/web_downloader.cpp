#include <nxi/web_downloader.hpp>

#include <nxi/session.hpp>

#include <QWebEngineDownloadRequest>

namespace nxi
{
    web_downloader::web_downloader(nxi::session& session)
        : session_{ session }
    {}

    void web_downloader::process(QWebEngineDownloadRequest* request)
    {
        request->setDownloadDirectory(session_.config().browser.download_path.get());
        request->accept();
        connect(request, &QWebEngineDownloadRequest::receivedBytesChanged, this, [request, this] {
            emit event_update((request->receivedBytes() * 100.f) / request->totalBytes());
        });
    }
} // nxi