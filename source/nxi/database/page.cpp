#include <nxi/database/page.hpp>

#include <nxi/core.hpp>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace nxi::data::page
{
    nxi::page_id add_page(nxi::core& core, const QString& name)
    {
        auto& query = core.database().prepared_query(nxi::prepared_query::add_page);
        query.bindValue(0, name);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
        return query.lastInsertId().toInt();
    }

    auto get_page(nxi::core& core, int id)
    {
        auto& query = core.database().prepared_query(nxi::prepared_query::get_page_id);
        query.bindValue(0, id);
        query.exec();
        //if (!query.next()) query.valu

        //nxi_model.page.name
    }

    //auto result = nxi::data::page::get_page(core, 3);
    //result[nxi_model.page.id]

    auto get_page(nxi::core& core, const QString& name)
    {
        auto& query = core.database().prepared_query(nxi::prepared_query::get_page_name);
        query.bindValue(0, name);
        query.exec();
        return query.next();
    }
} // nxi::data::page

namespace nxi::data::page::internal
{
    void make(nxi::database& db)
    {
        db.query(dbs::core, R"__(
        CREATE TABLE `page`
        (
            `id` integer,
            `name` text(8),
            `command` text(8),
            `type` integer,
            `renderer_type` integer,
            `loaded` integer,
            `muted` integer,
            PRIMARY KEY(`id`)
        )
        )__");
    }

    void prepare(nxi::database& db)
    {
        db.prepare(dbs::core, prepared_query::add_page, "INSERT INTO page(name) VALUES(?)");
        db.prepare(dbs::core, prepared_query::get_page_id, "SELECT * FROM page WHERE page.id = ?");
        db.prepare(dbs::core, prepared_query::get_page_name, "SELECT * FROM page WHERE page.name = ?");
    }
} // nxi::data::page::internal