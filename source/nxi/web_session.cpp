#include <nxi/web_session.hpp>

#include <nxi/data/cookie.hpp>
#include <nxi/user_session.hpp>

#include <nxi/database/model.hpp>
#include <QNetworkCookie>
#include <QSqlDatabase>
#include <QSqlError>
#include <QWebEngineCookieStore>
#include <QWebEngineNotification>
#include <QWebEngineProfile>
#include <QWebEngineSettings>

namespace nxi
{
    web_session::web_session(nxi::user_session& session, QObject* parent)
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

        connect(cookieStore(), &QWebEngineCookieStore::cookieAdded, [this](const QNetworkCookie& cookie)
        {
            nxi::data::cookie::set(session_.database(), cookie);
        });

        connect(cookieStore(), &QWebEngineCookieStore::cookieRemoved, [this](const QNetworkCookie& cookie)
        {
            nxi::data::cookie::del(session_.database(), cookie);
        });

        settings()->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled, true);
        settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);
        setNotificationPresenter([this](std::unique_ptr<QWebEngineNotification> notif) { session_.error(notif->message()); });
        setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:99.0) Gecko/20100101 Firefox/99.0");
    }

    void web_session::load_cookie(const QString& domain)
    {
        QString out = domain;
        if (domain.count('.') > 1)
        {
            int first = domain.lastIndexOf(".");
            if (first > 0)
            {
                int second = domain.mid(0, first).lastIndexOf(".");
                if (second > 0) out = domain.mid(second + 1);
            }
        }

        auto result = nxi::data::cookie::get(session_.database(), "%" + out);
        while(result.next())
        {
            QNetworkCookie cookie = nxi::data::cookie::make(nxi::data::cookie::from_get(result));
            cookieStore()->setCookie(cookie);
        }
    }

    void web_session::import_cookies(const QString& database_path)
    {
        //C:\Users\ads\AppData\Roaming\Mozilla\Firefox\Profiles\y9siacyv.default-1475306262438\cookies.sqlite
        auto import_db = QSqlDatabase::addDatabase("QSQLITE", database_path);
        import_db.setDatabaseName(database_path);

        if(!import_db.open())
        {
            nxi_error("import_cookies failed connection error : {}", import_db.lastError().text());
            return;
        }

        QSqlQuery query{ import_db };
        query.exec("SELECT host, expiry, isHttpOnly, name, path, sameSite, isSecure, value FROM moz_cookies");

        while (query.next())
        {
            auto domain = query.value(0).toString();
            auto expiry = query.value(1).toLongLong();
            auto isHttpOnly = query.value(2).toBool();
            auto name = query.value(3).toByteArray();
            auto path = query.value(4).toString();
            auto sameSite = query.value(5).toInt();
            auto isSecure = query.value(6).toBool();
            auto value = query.value(7).toByteArray();
            QNetworkCookie cookie;

            cookie.setDomain(domain);
            cookie.setExpirationDate(QDateTime::fromSecsSinceEpoch(expiry));
            cookie.setHttpOnly(isHttpOnly);
            cookie.setName(name);
            cookie.setPath(path);
            //cookie.setSameSitePolicy(QNetworkCookie::SameSite::);
            cookie.setSecure(isSecure);
            cookie.setValue(value);
            nxi::data::cookie::set(session_.database(), cookie);
            cookieStore()->setCookie(cookie);
        }
    }
} // nxi