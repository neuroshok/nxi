#include <nxi/data/cookie.hpp>

#include <nxi/core.hpp>
#include <nxi/database/model.hpp>
#include <nxi/database/result.hpp>

#include <QNetworkCookie>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace nxi
{
    nxi::cookie_data cookie_data::from_get(const nxi::result& result)
    {
        nxi::cookie_data data;

        //data.name = result[nxi_model.module.name];

        return data;
    }
} // nxi

namespace nxi::data::cookie
{
    nxi::result get(nxi::database& db)
    {
        auto& query = db.prepared_query(nxi::prepared_query::get_cookie);
        return nxi::result{ query };
    }

    nxi::result get(nxi::database& db, const QString& domain)
    {
        auto& query = db.prepared_query(nxi::prepared_query::get_cookie_domain);
        query.bindValue(0, domain);
        return nxi::result{ query };
    }

    void del(nxi::database& db, const QNetworkCookie& cookie)
    {
        auto& query = db.prepared_query(nxi::prepared_query::del_cookie);
        query.bindValue(0, cookie.name());
        query.bindValue(1, cookie.domain());
        query.bindValue(2, cookie.path());
        db.exec(query);
    }

    void set(database& db, const QNetworkCookie& cookie)
    {
        auto& query = db.prepared_query(nxi::prepared_query::set_cookie);
        query.bindValue(0, cookie.domain());
        query.bindValue(1, cookie.expirationDate().toSecsSinceEpoch());
        query.bindValue(2, cookie.isHttpOnly());
        query.bindValue(3, cookie.name());
        query.bindValue(4, cookie.path());
        query.bindValue(5, static_cast<int>(cookie.sameSitePolicy()));
        query.bindValue(6, cookie.isSecure());
        query.bindValue(7, cookie.value());
        db.exec(query);
    }

    nxi::cookie_data from_get(const nxi::result& result)
    {
        nxi::cookie_data data;
        data.id = result[nxi_model.cookie.id];
        data.domain = result[nxi_model.cookie.domain];
        data.expiration = result[nxi_model.cookie.expiration];
        data.http_only = result[nxi_model.cookie.http_only];
        data.name = result[nxi_model.cookie.name];
        data.path = result[nxi_model.cookie.path];
        data.same_site = result[nxi_model.cookie.same_site];
        data.secured = result[nxi_model.cookie.secured];
        data.value = result[nxi_model.cookie.value];
        return data;
    }

    QNetworkCookie make(const nxi::cookie_data& data)
    {
        QNetworkCookie cookie;
        cookie.setDomain(data.domain);
        cookie.setExpirationDate(QDateTime::fromSecsSinceEpoch(data.expiration));
        cookie.setHttpOnly(data.http_only);
        cookie.setName(data.name);
        cookie.setPath(data.path);
        cookie.setSameSitePolicy(static_cast<QNetworkCookie::SameSite>(data.same_site));
        cookie.setSecure(data.secured);
        cookie.setValue(data.value);

        return cookie;
    }
} // nxi::data::cookie

namespace nxi::data::cookie::internal
{
    void prepare(nxi::database& db)
    {
        db.prepare(prepared_query::del_cookie, "DELETE FROM cookie WHERE name = ? AND domain = ? AND path = ?");
        db.prepare(prepared_query::get_cookie, "SELECT * FROM cookie");
        db.prepare(prepared_query::get_cookie_domain, "SELECT * FROM cookie WHERE domain LIKE ?");
        db.prepare(prepared_query::set_cookie, "INSERT OR REPLACE INTO cookie"
                                               "(domain, expiration, http_only, name, path, same_site, secured, value) "
                                               "VALUES(?, ?, ?, ?, ?, ?, ?, ?)");
    }
} // nxi::data::page::internal