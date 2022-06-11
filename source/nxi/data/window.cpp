#include <nxi/data/window.hpp>

#include <nxi/database.hpp>

namespace nxi::data::window
{
    unsigned int add_window(nxi::database& db, nxi::window_data window)
    {
        auto& query = db.prepared_query(nxi::prepared_query::add_window);
        query.bindValue(0, window.x);
        query.bindValue(1, window.y);
        query.bindValue(2, window.w);
        query.bindValue(3, window.h);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
        return query.lastInsertId().toInt();
    }

    nxi::result get_windows(nxi::database& db)
    {
        auto& query = db.prepared_query(nxi::prepared_query::get_windows);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
        return nxi::result{ query };
    }

    void move_window(nxi::database& db, unsigned int id, int x, int y)
    {
        auto& query = db.prepared_query(nxi::prepared_query::move_window);
        query.bindValue(0, x);
        query.bindValue(1, y);
        query.bindValue(2, id);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
    }

    void del_window(nxi::database& db, int id)
    {
        auto& query = db.prepared_query(nxi::prepared_query::del_window);
        query.bindValue(0, id);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
    }
} // nxi::data::window

namespace nxi::data::window::internal
{
    void prepare(nxi::database& db)
    {
        db.prepare(prepared_query::add_window, "INSERT INTO window(x, y, w, h) VALUES(?, ?, ?, ?)");
        db.prepare(prepared_query::del_window, "DELETE FROM window WHERE id = ?");
        db.prepare(prepared_query::get_windows, "SELECT id, x, y, w, h FROM window");
        db.prepare(prepared_query::move_window, "UPDATE window SET x = ?, y = ? WHERE id = ?");
    }
} // nxi::data::page::internal