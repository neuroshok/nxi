#include <nxi/web_session.hpp>

#include <nxi/session.hpp>

#include <QNetworkCookie>
#include <QSqlDatabase>
#include <QSqlError>
#include <QWebEngineCookieStore>
#include <QWebEngineNotification>
#include <QWebEngineProfile>
#include <QWebEngineSettings>

namespace nxi
{
    web_session::web_session(nxi::session& session, QObject* parent)
        : QWebEngineProfile(parent)
        , session_{ session }
    {}

    void web_session::load()
    {
        cookie_db_ = QSqlDatabase::addDatabase("QSQLITE", nxi::database::path + QString{ "cookie" });

        //setCachePath("./cache");
        //setPersistentStoragePath("./cache");
        setHttpCacheType(QWebEngineProfile::HttpCacheType::DiskHttpCache);
        setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);

        connect(cookieStore(), &QWebEngineCookieStore::cookieAdded, [this](const QNetworkCookie& c)
        {
            //add_cookie(cookie); nxi::data::cookie::set(cookie);
        });

        connect(cookieStore(), &QWebEngineCookieStore::cookieRemoved, [this](const QNetworkCookie& c)
        {
            //cookieStore()->deleteCookie(cookie)  nxi::data::cookie::del(cookie);
        });

        settings()->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled, true);
        settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);
        setNotificationPresenter([this](std::unique_ptr<QWebEngineNotification> notif) { session_.error(notif->message()); });
        setHttpUserAgent("Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:10.0) Gecko/20100101 Firefox/99.0.1");
    }

    void web_session::import_cookies(const QString& database_path)
    {
        auto import_db = QSqlDatabase::addDatabase("QSQLITE", database_path);
        import_db.setDatabaseName(database_path);

        if(!import_db.open())
        {
            nxi_error("import_cookies failed connection error : {}", import_db.lastError().text());
            return;
        }

        QSqlQuery query{ import_db };
        if (!query.exec("SELECT host, path, name, value FROM moz_cookies")) nxi_error("query error : {}", query.lastError().text());

        while (query.next())
        {
            auto host = query.value(0).toString();
            auto path = query.value(1).toString();
            auto name = query.value(2).toByteArray();
            auto value = query.value(3).toByteArray();
            QNetworkCookie cookie;
            cookie.setDomain(host);
            cookie.setPath(path);
            cookie.setName(name);
            cookie.setValue(value);
            cookieStore()->setCookie(cookie);
            // nxi::data::cookie::set(cookie);
        }
    }

} // nxi