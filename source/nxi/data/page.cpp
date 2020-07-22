#include <nxi/data/page.hpp>

#include <nxi/core.hpp>
#include <nxi/page.hpp>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace nxi::data::page
{
    nxi::page_id add(nxi::database& db, nxi::page& page)
    {
        auto& query = db.prepared_query(nxi::prepared_query::add_page);
        query.bindValue(0, page.name());
        query.bindValue(1, page.command());
        query.bindValue(2, static_cast<int>(page.type()));
        query.bindValue(3, static_cast<int>(page.renderer_type()));
        query.bindValue(4, page.is_loaded());
        query.bindValue(5, page.is_muted());
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
        page.id_ = query.lastInsertId().toInt();
        return page.id();
    }

    size_t count(nxi::database& db)
    {
        auto& query = db.prepared_query(nxi::prepared_query::count_page);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
        query.next();
        return query.value(0).toUInt();
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

    void add_arc(database& db, int source_id, int target_id)
    {
        auto& query = db.prepared_query(nxi::prepared_query::add_page_arc);
        query.bindValue(0, source_id);
        query.bindValue(1, target_id);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
    }
} // nxi::data::page

namespace nxi::data::page::internal
{
    void make(nxi::database& db)
    {
        db.query(internal::str_table_page.data());
        db.query(internal::str_table_page_arc.data());
    }

    void prepare(nxi::database& db)
    {
        db.prepare(prepared_query::add_page, "INSERT INTO page(name, command, type, renderer_type, loaded, muted) VALUES(?, ?, ?, ?, ?, ?)");
        db.prepare(prepared_query::add_page_arc, "INSERT INTO page_arc(source_id, target_id) VALUES(?, ?)");
        db.prepare(prepared_query::count_page, "SELECT COUNT(id) FROM page");
        db.prepare(prepared_query::get_page_id, "SELECT id, name, command FROM page WHERE page.id = ?");
        db.prepare(prepared_query::get_page_name, "SELECT id, name, command FROM page WHERE page.name = ?");
    }
} // nxi::data::page::internal