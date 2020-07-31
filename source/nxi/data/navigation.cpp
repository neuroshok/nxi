#include <nxi/data/navigation.hpp>

#include <nxi/database.hpp>
#include <nxi/log.hpp>

#include <QVariant>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace nxi::data::navigation
{
    void log(nxi::database& db, nxi::page_id page_id, const QString& source, const QString& target, int type)
    {
        auto& query = db.prepared_query(nxi::prepared_query::navigation_log_add);
        query.bindValue(0, page_id);
        query.bindValue(1, source);
        query.bindValue(2, target);
        query.bindValue(3, type);
        if (!query.exec()) nxi_error("query error : {}", query.lastError().text());
    }
} // nxi::data::navigation

namespace nxi::data::navigation::internal
{
    void make(nxi::database& db)
    {
        db.query(internal::str_table_navigation_log.data());
    }

    void prepare(nxi::database& db)
    {
        db.prepare(nxi::prepared_query::navigation_log_add, "INSERT INTO navigation_log(datetime, page_id, source, target, type) VALUES(datetime('now'), ?, ?, ?, ?)");
    }
}
// nxi::data::navigation::internal