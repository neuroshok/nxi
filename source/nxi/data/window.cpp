#include <nxi/data/window.hpp>

#include <nxi/core.hpp>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace nxi::data::window
{
    unsigned int add_window(nxi::core& core, nxi::window_data window)
    {
        auto& query = core.database().prepared_query(nxi::prepared_query::add_window);
        query.bindValue(0, window.x);
        query.bindValue(1, window.y);
        query.bindValue(2, window.w);
        query.bindValue(3, window.h);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
        return query.lastInsertId().toInt();
    }

    nxi::result get_windows(nxi::core& core)
    {
        auto& query = core.database().prepared_query(nxi::prepared_query::get_windows);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
        return nxi::result{ query };
    }

    void move_window(nxi::core& core, unsigned int id, int x, int y)
    {
        auto& query = core.database().prepared_query(nxi::prepared_query::move_window);
        query.bindValue(0, x);
        query.bindValue(1, y);
        query.bindValue(2, id);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
    }
} // nxi::data::window

namespace nxi::data::window::internal
{
    void make(nxi::database& db)
    {
        db.query(internal::str_table.data());
    }

    void prepare(nxi::database& db)
    {
        db.prepare(prepared_query::add_window, "INSERT INTO window(x, y, w, h) VALUES(?, ?, ?, ?)");
        db.prepare(prepared_query::get_windows, "SELECT id, x, y, w, h FROM window");
        db.prepare(prepared_query::move_window, "UPDATE window SET x = ?, y = ? WHERE id = ?");
    }
} // nxi::data::page::internal