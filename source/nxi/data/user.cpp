#include <nxi/data/user.hpp>

#include <nxi/core.hpp>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace nxi::data::user
{
    unsigned int add(nxi::database& db, const nxi::user_data& data)
    {
        auto& query = db.prepared_query(nxi::prepared_query::add_user);
        query.bindValue(0, data.name);
        query.bindValue(1, data.active);
        query.bindValue(2, data.focus);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
        return query.lastInsertId().toInt();
    }

    void del(nxi::database& db, int id)
    {
        auto& query = db.prepared_query(nxi::prepared_query::del_user);
        query.bindValue(0, id);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
    }

    nxi::result get(nxi::database& db)
    {
        auto& query = db.prepared_query(nxi::prepared_query::get_users);
        return nxi::result{ query };
    }

    void load(nxi::database& db, int id)
    {
        auto& query = db.prepared_query(nxi::prepared_query::load_user);
        query.bindValue(0, id);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
    }

    void unload(nxi::database& db, int id)
    {
        auto& query = db.prepared_query(nxi::prepared_query::unload_user);
        query.bindValue(0, id);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
    }
} // nxi::data::user

namespace nxi::data::user::internal
{
    void prepare(nxi::database& db)
    {
        db.prepare(prepared_query::add_user, "INSERT INTO user(name, active, focus) VALUES(?, ?, ?)");
        db.prepare(prepared_query::del_user, "DELETE FROM user WHERE id = ?");
        db.prepare(prepared_query::get_users, "SELECT id, name, active, focus FROM user");
        db.prepare(prepared_query::load_user, "UPDATE user SET active = 1 WHERE id = ?");
        db.prepare(prepared_query::unload_user, "UPDATE user SET active = 0 WHERE id = ?");
    }
} // nxi::data::user::internal