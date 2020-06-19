#include <nxi/database.hpp>

#include <nxi/data/page.hpp>
#include <nxi/log.hpp>

#include <filesystem>

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

#include <qdebug.h>

namespace fs = std::filesystem;

namespace nxi
{
    database::database()
        : make_required_{ false }
    {
        if (!fs::exists(path.toStdString()))
        {
            fs::create_directory(path.toStdString());
            make_required_ = true;
        }

        databases_[static_cast<size_t>(dbs::core)] = QSqlDatabase::addDatabase("QSQLITE", "core");
        get(dbs::core).setDatabaseName(path + "core");
        databases_[static_cast<size_t>(dbs::module)] = QSqlDatabase::addDatabase("QSQLITE", "module");
        get(dbs::module).setDatabaseName(path + "module");
    }

    void database::connect(nxi::dbs db_id)
    {
        auto& db = get(db_id);
        nxi_trace("connect to database {}", db.databaseName());

        if(!db.open()) nxi_error("connection error : {}", db.lastError().text());

        if (make_required_) make(db_id);

        nxi::data::page::internal::prepare(*this);
    }

    QSqlDatabase& database::get(nxi::dbs db_id)
    {
        nxi_assert(static_cast<size_t>(db_id) < static_cast<size_t>(dbs::size_));
        return databases_[static_cast<size_t>(db_id)];
    }

    void database::make(nxi::dbs db_id)
    {
        nxi::data::page::internal::make(*this);
    }

    void database::prepare(nxi::dbs db_id, nxi::prepared_query pquery, const QString& str_query)
    {
        QSqlQuery query{ get(db_id) };
        query.prepare(str_query);
        prepared_query(pquery) = std::move(query);

    }

    QSqlQuery& database::prepared_query(nxi::prepared_query pquery)
    {
        return queries_[static_cast<size_t>(pquery)];
    }

    void database::query(nxi::dbs db_id, const QString& str_query)
    {
        auto& db = get(db_id);
        QSqlQuery query{ db };
        if (!query.exec(str_query)) nxi_error("query error : {}", query.lastError().text());
    }
} // nxi
