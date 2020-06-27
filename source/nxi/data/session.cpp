#include <nxi/data/session.hpp>

#include <nxi/core.hpp>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace nxi::data::session
{
    unsigned int add_session(nxi::core& core, const QString& name)
    {
        auto& query = core.global_database().prepared_query(nxi::prepared_query::add_session);
        query.bindValue(0, name);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
        return query.lastInsertId().toInt();
    }

    nxi::result get_sessions(nxi::core& core)
    {
        auto& query = core.global_database().prepared_query(nxi::prepared_query::get_sessions);
        return nxi::result{ query };
    }
} // nxi::data::session

namespace nxi::data::session::internal
{
    void make(nxi::database& db)
    {
        db.query(internal::str_table.data());
    }

    void prepare(nxi::database& db)
    {
        db.prepare(prepared_query::add_session, "INSERT INTO session(name, active) VALUES(?, 1)");
        db.prepare(prepared_query::get_sessions, "SELECT name, active FROM session");
    }
} // nxi::data::session::internal