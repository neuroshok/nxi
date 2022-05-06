#include <nxi/data/session.hpp>

#include <nxi/database.hpp>

namespace nxi::data::session
{
    unsigned int add(nxi::database& db, const nxi::session_data& data)
    {
        auto& query = db.prepared_query(nxi::prepared_query::add_session);
        query.bindValue(0, data.name);
        query.bindValue(1, data.active);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
        return query.lastInsertId().toInt();
    }

    void del(nxi::database& db, int id)
    {
        auto& query = db.prepared_query(nxi::prepared_query::del_session);
        query.bindValue(0, id);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
    }

    nxi::result get(nxi::database& db)
    {
        auto& query = db.prepared_query(nxi::prepared_query::get_sessions);
        return nxi::result{ query };
    }

    void load(nxi::database& db, int id)
    {
        auto& query = db.prepared_query(nxi::prepared_query::load_session);
        query.bindValue(0, id);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
    }

    void unload(nxi::database& db, int id)
    {
        auto& query = db.prepared_query(nxi::prepared_query::unload_session);
        query.bindValue(0, id);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
    }
} // nxi::data::session

namespace nxi::data::session::internal
{
    void prepare(nxi::database& db)
    {
        db.prepare(prepared_query::add_session, "INSERT INTO session(name, active) VALUES(?, ?)");
        db.prepare(prepared_query::del_session, "DELETE FROM session WHERE id = ?");
        db.prepare(prepared_query::get_sessions, "SELECT id, name, active FROM session");
        db.prepare(prepared_query::load_session, "UPDATE session SET active = 1 WHERE id = ?");
        db.prepare(prepared_query::unload_session, "UPDATE session SET active = 0 WHERE id = ?");
    }
} // nxi::data::session::internal