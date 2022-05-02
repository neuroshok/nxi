#include <nxi/data/context.hpp>

#include <nxi/core.hpp>
#include <nxi/database/model.hpp>
#include <nxi/database/result.hpp>
#include <nxi/page.hpp>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace nxi
{
    nxi::context_data context_data::from_get(const nxi::result& result)
    {
        nxi::context_data data;
        data.name = result[nxi_model.context.name];
        data.enabled = result[nxi_model.context.enabled];
        data.priority = result[nxi_model.context.priority];
        return data;
    }
} // nxi

namespace nxi::data::context
{
    void add(nxi::database& db, const nxi::context& context)
    {
        auto& query = db.prepared_query(nxi::prepared_query::add_context);
        query.bindValue(0, context.id());
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
    }

    void add_available(nxi::database& db , const nxi::context_data& context)
    {
        auto& query = db.prepared_query(nxi::prepared_query::add_context_available);
        query.bindValue(0, context.name);
        query.bindValue(1, context.priority);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
    }

    void del(nxi::database& db, const nxi::context& context)
    {
        auto& query = db.prepared_query(nxi::prepared_query::del_context);
        query.bindValue(0, context.id());
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
    }

    nxi::result get(nxi::database& db)
    {
        auto& query = db.prepared_query(nxi::prepared_query::get_context);
        return nxi::result{ query };
    }

    nxi::result get_available(nxi::database& db)
    {
        auto& query = db.prepared_query(nxi::prepared_query::get_context_available);
        return nxi::result{ query };
    }

} // nxi::data::context

namespace nxi::data::context::internal
{
    void make(nxi::database& db)
    {
        db.exec(internal::str_table_context.data());
    }

    void prepare(nxi::database& db)
    {
        db.prepare(prepared_query::add_context, "UPDATE context SET enabled = 1 WHERE name = ?");
        db.prepare(prepared_query::add_context_available, "INSERT INTO context(name, priority) VALUES(?, ?)");
        db.prepare(prepared_query::del_context, "UPDATE context SET enabled = 0 WHERE name = ?");
        db.prepare(prepared_query::get_context, "SELECT * FROM context WHERE enabled = 1");
        db.prepare(prepared_query::get_context_available, "SELECT * FROM context");
    }
} // nxi::data::context::internal