#ifndef INCLUDE_NXI_WEB_DOWNLOADER_HPP_NXI
#define INCLUDE_NXI_WEB_DOWNLOADER_HPP_NXI

#include <QWidget>

class QWebEngineDownloadRequest;

namespace nxi
{
    class session;

    class web_downloader : public QObject
    {
        Q_OBJECT
    public:
        explicit web_downloader(nxi::session&);

        void process(QWebEngineDownloadRequest*);

    signals:
        void event_update(float percent);

    private:
        nxi::session& session_;
    };
} // nxi

#endif // INCLUDE_NXI_WEB_DOWNLOADER_HPP_NXI