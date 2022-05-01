#ifndef INCLUDE_WEB_SESSION_HPP_NXI
#define INCLUDE_WEB_SESSION_HPP_NXI

#include <QSqlDatabase>
#include <QWebEngineProfile>

namespace nxi
{
    class session;

    class web_session : public QWebEngineProfile
    {
    public:
        web_session(nxi::session&, QObject* = nullptr);

        void load();

        void import_cookies(const QString& database_path);

    private:
        nxi::session& session_;

        QSqlDatabase cookie_db_;
    };
} // nxi

#endif // INCLUDE_WEB_SESSION_HPP_NXI