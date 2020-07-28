#include <nxi/data/page.hpp>

#include <nxi/core.hpp>
#include <nxi/database/model.hpp>
#include <nxi/database/result.hpp>
#include <nxi/page.hpp>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace nxi
{
    nxi::page_data page_data::from_get(const nxi::result& result)
    {
        nxi::page_data data;
        data.command = result[nxi_model.page.command];
        data.id = result[nxi_model.page.id];
        data.loaded = result[nxi_model.page.loaded];
        data.muted = result[nxi_model.page.muted];
        data.name = result[nxi_model.page.name];
        data.renderer_type = static_cast<nxi::renderer_type>(result[nxi_model.page.renderer_type]);
        data.type = static_cast<nxi::page_type>(result[nxi_model.page.type]);
        return data;
    }
} // nxi

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

    nxi::result get(nxi::database& db)
    {
        auto& query = db.prepared_query(nxi::prepared_query::get_page);
        return nxi::result{ query };
    }

    nxi::result get(nxi::database& db, int id)
    {
        auto& query = db.prepared_query(nxi::prepared_query::get_page_id);
        query.bindValue(0, id);

        return nxi::result{ query };
    }

    nxi::result get(nxi::database& db, const QString& name)
    {
        auto& query = db.prepared_query(nxi::prepared_query::get_page_name);
        query.bindValue(0, name);

        return nxi::result{ query };
    }

    void del(nxi::database& db, nxi::page& page)
    {
        auto& query = db.prepared_query(nxi::prepared_query::del_page);
        query.bindValue(0, page.id());
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
    }

    void set_loaded(database& db, nxi::page_id id, bool state)
    {
        auto& query = db.prepared_query(nxi::prepared_query::set_page_loaded);
        query.bindValue(0, state);
        query.bindValue(1, id);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
    }

    //

    void add_arc(nxi::database& db, int source_id, int target_id)
    {
        auto& query = db.prepared_query(nxi::prepared_query::add_page_arc);
        query.bindValue(0, source_id);
        query.bindValue(1, target_id);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
    }

    nxi::result get_arcs(nxi::database& db)
    {
        auto& query = db.prepared_query(nxi::prepared_query::get_page_arcs);
        return nxi::result{ query };
    }

    void del_arc(nxi::database& db, int page_id)
    {
        auto& query = db.prepared_query(nxi::prepared_query::del_page_arc);
        query.bindValue(0, page_id);
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
        db.prepare(prepared_query::del_page, "DELETE FROM page WHERE id = ?");
        db.prepare(prepared_query::count_page, "SELECT COUNT(id) FROM page");
        db.prepare(prepared_query::get_page, "SELECT * FROM page");
        db.prepare(prepared_query::get_page_id, "SELECT id, name, command FROM page WHERE page.id = ?");
        db.prepare(prepared_query::get_page_name, "SELECT id, name, command FROM page WHERE page.name = ?");
        db.prepare(prepared_query::set_page_loaded, "UPDATE page SET loaded = ? WHERE id = ?");

        db.prepare(prepared_query::add_page_arc, "INSERT INTO page_arc(source_id, target_id) VALUES(?, ?)");
        db.prepare(prepared_query::del_page_arc, "DELETE FROM page_arc WHERE source_id = $1 OR target_id = $1");
        db.prepare(prepared_query::get_page_arcs, "SELECT * FROM page_arc");
    }
} // nxi::data::page::internal