#include <nxi/data/page.hpp>

#include <nxi/core.hpp>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace nxi::data::page
{
    nxi::page_id add_page(nxi::database& db, const QString& name)
    {
        auto& query = db.prepared_query(nxi::prepared_query::add_page);
        query.bindValue(0, name);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
        return query.lastInsertId().toInt();
    }

    nxi::result get_page(nxi::database& db, int id)
    {
        auto& query = db.prepared_query(nxi::prepared_query::get_page_id);
        query.bindValue(0, id);

        return nxi::result{ query };
    }

    nxi::result get_page(nxi::database& db, const QString& name)
    {
        auto& query = db.prepared_query(nxi::prepared_query::get_page_name);
        query.bindValue(0, name);

        return nxi::result{ query };
    }
} // nxi::data::page

namespace nxi::data::page::internal
{
    void make(nxi::database& db)
    {
        db.query(internal::str_table.data());
    }

    void prepare(nxi::database& db)
    {
        db.prepare(prepared_query::add_page, "INSERT INTO page(name) VALUES(?)");
        db.prepare(prepared_query::get_page_id, "SELECT id, name, command FROM page WHERE page.id = ?");
        db.prepare(prepared_query::get_page_name, "SELECT id, name, command FROM page WHERE page.name = ?");
    }
} // nxi::data::page::internal